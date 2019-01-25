#include "gamemenuhandler.h"
#include "minemenu.h"
#include "town.h"


MineMenu::MineMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    nameOfMenu->setPixmap(QPixmap(":images/images/Mine_Menu/mMine.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem();
    m1->setPixmap(QPixmap(":images/images/Mine_Menu/m1.png"));
    menuItems.push_back(m1);
}

void MineMenu::processSelectAction(const int & currentItem)
{
    if(currentItem == -1)
        return;
    if(currentItem == 0)
        emit M_LevelUp();
}

void MineMenu::connectWithObject(const QObject * objectForConnect) const
{
    if(typeid(*objectForConnect) == typeid(Town))
    {
        connect(this, &MineMenu::M_LevelUp, dynamic_cast<const Town *>(objectForConnect), &Town::setNewIncome);
    }
}

int MineMenu::getPriceOfCurrentItem(const QMap<QString, int> * map, const int & currentItem) const
{
    if(currentItem == -1)
        return 0;
    if(currentItem == 0)
        return map->value("Mine level up");
}

QVector<int> MineMenu::restoreLastGame(const QMap<QString, int> & rpum) const
{
    QVector<int> vec;
    if(!rpum.contains("Mine level up"))
        vec.append(0);

    return vec;
}
