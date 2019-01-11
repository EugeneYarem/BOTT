#include "gamemenuhandler.h"
#include "magicmenu.h"
#include "Military/army.h"


MagicMenu::MagicMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    nameOfMenu->setPixmap(QPixmap(":images/images/Magic_Menu/mMagic.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem(),
                        * m2 = new QGraphicsPixmapItem();

    m1->setPixmap(QPixmap(":images/images/Magic_Menu/m1.png"));
    m2->setPixmap(QPixmap(":images/images/Magic_Menu/m2.png"));
    menuItems.push_back(m1);
    menuItems.push_back(m2);
}

void MagicMenu::processSelectAction(int currentItem)
{
    if(currentItem == -1)
        return;
    if(currentItem == 0)
        emit M_MageAttack();
    if(currentItem == 1)
        emit M_MageHealth();
}

void MagicMenu::connectWithObject(QObject * objectForConnect)
{
    if(typeid(*objectForConnect) == typeid(Army))
    {
        connect(this, &MagicMenu::M_MageHealth, dynamic_cast<Army *>(objectForConnect), &Army::increaseMageHitPoint);
        connect(this, &MagicMenu::M_MageAttack, dynamic_cast<Army *>(objectForConnect), &Army::increaseMageAttack);
    }
}

int MagicMenu::getPriceOfCurrentItem(QMap<QString, int> * map, int currentItem)
{
    if(currentItem == -1)
        return 0;
    if(currentItem == 0)
        return map->value("Mage_Attack");
    if(currentItem == 1)
        return map->value("Mage_Hp");
}
