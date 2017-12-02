#include "sciencemenu.h"
#include "gamemenuhandler.h"
#include "Military/army.h"

ScienceMenu::ScienceMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    nameOfMenu->setPixmap(QPixmap(":images/images/Science_Menu/mScienceCentre.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem(),
                        * m2 = new QGraphicsPixmapItem();

    m1->setPixmap(QPixmap(":images/images/Science_Menu/sc1.png"));
    m2->setPixmap(QPixmap(":images/images/Science_Menu/sc2.png"));
    menuItems.push_back(m1);
    menuItems.push_back(m2);
}

void ScienceMenu::processSelectAction(int currentItem)
{
    if(currentItem == -1)
        return;
    if(currentItem == 0)
        emit SC_MageAttack();
    if(currentItem == 1)
        emit SC_MageHealth();
}

void ScienceMenu::connectWithObject(QObject * objectForConnect)
{
    if(typeid(*objectForConnect) == typeid(Army))
    {
        connect(this, SIGNAL(SC_MageHealth()), (Army *)(objectForConnect), SLOT(increaseMageHitPoint()));
        connect(this, SIGNAL(SC_MageAttack()), (Army *)(objectForConnect), SLOT(increaseMageAttack()));
    }
}

int ScienceMenu::getPriceOfCurrentItem(QMap<QString, int> * map, int currentItem)
{
    if(currentItem == -1)
        return 0;
    if(currentItem == 0)
        return map->value("Mage_Attack");
    if(currentItem == 1)
        return map->value("Mage_Hp");
}
