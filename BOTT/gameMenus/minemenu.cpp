#include "minemenu.h"
#include "gamemenuhandler.h"
#include "Military/army.h"
#include "town.h"
#include <QDebug>

MineMenu::MineMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    nameOfMenu->setPixmap(QPixmap(":images/images/Mine_Menu/mMine.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem();
    m1->setPixmap(QPixmap(":images/images/Mine_Menu/m1.png"));
    menuItems.push_back(m1);
}

void MineMenu::processSelectAction(int currentItem)
{
    if(currentItem == -1)
        return;
    if(currentItem == 0)
        emit M_LevelUp();
}

void MineMenu::connectWithObject(QObject * objectForConnect)
{
    if(typeid(*objectForConnect) == typeid(Town))
    {
        connect(this, SIGNAL(M_LevelUp()), (Town *)objectForConnect, SLOT(setNewIncome()));
    }
}

int MineMenu::getPriceOfCurrentItem(QMap<QString, int> * map, int currentItem)
{
    if(currentItem == -1)
        return 0;
    if(currentItem == 0)
        return map->value("Mine level up");
}
