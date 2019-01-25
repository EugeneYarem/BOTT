#include "gamemenuhandler.h"
#include "Military/army.h"
#include "workshopmenu.h"


WorkshopMenu::WorkshopMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    nameOfMenu->setPixmap(QPixmap(":images/images/Workshop_Menu/mWorkshop.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem(),
                        * m2 = new QGraphicsPixmapItem(),
                        * m3 = new QGraphicsPixmapItem(),
                        * m4 = new QGraphicsPixmapItem();

    m1->setPixmap(QPixmap(":images/images/Workshop_Menu/w1.png"));
    m2->setPixmap(QPixmap(":images/images/Workshop_Menu/w2.png"));
    m3->setPixmap(QPixmap(":images/images/Workshop_Menu/w3.png"));
    m4->setPixmap(QPixmap(":images/images/Workshop_Menu/w4.png"));
    menuItems.push_back(m1);
    menuItems.push_back(m2);
    menuItems.push_back(m3);
    menuItems.push_back(m4);
}

void WorkshopMenu::processSelectAction(const int & currentItem)
{
    if(currentItem == -1)
        return;
    if(currentItem == 0)
        emit W_MakeArquebus();
    if(currentItem == 1)
        emit W_HauberkExplored();
    if(currentItem == 2)
        emit W_ArmorExplored();
    if(currentItem == 3)
        emit W_WeaponLevelUp();
}

void WorkshopMenu::connectWithObject(const QObject * objectForConnect) const
{
    if(typeid(*objectForConnect) == typeid(Army))
    {    
        connect(this, &WorkshopMenu::W_MakeArquebus, dynamic_cast<const Army *>(objectForConnect), &Army::improveArquebus);
        connect(this, &WorkshopMenu::W_MakeArquebusInLastGame, dynamic_cast<const Army *>(objectForConnect), &Army::setArquebusImprove);
        connect(this, &WorkshopMenu::W_HauberkExplored, dynamic_cast<const Army *>(objectForConnect), &Army::improveHauberk);
        connect(this, &WorkshopMenu::W_HauberkExploredInLastGame, dynamic_cast<const Army *>(objectForConnect), &Army::setHauberkImprove);
        connect(this, &WorkshopMenu::W_ArmorExplored, dynamic_cast<const Army *>(objectForConnect), &Army::improveArmor);
        connect(this, &WorkshopMenu::W_ArmorExploredInLastGame, dynamic_cast<const Army *>(objectForConnect), &Army::setArmorImprove);
        connect(this, &WorkshopMenu::W_WeaponLevelUp, dynamic_cast<const Army *>(objectForConnect), &Army::improveWeapon);
        connect(this, &WorkshopMenu::W_WeaponLevelUpInLastGame, dynamic_cast<const Army *>(objectForConnect), &Army::setWeaponImprove);
    }
}

int WorkshopMenu::getPriceOfCurrentItem(const QMap<QString, int> * map, const int & currentItem) const
{
    if(currentItem == -1)
        return 0;
    if(currentItem == 0)
        return map->value("Arquebus");
    if(currentItem == 1)
        return map->value("Hauberk");
    if(currentItem == 2)
        return map->value("Armor");
    if(currentItem == 3)
        return map->value("Weapon");
}

QVector<int> WorkshopMenu::restoreLastGame(const QMap<QString, int> & rpum) const
{
    QVector<int> vec;
    if(!rpum.contains("Arquebus"))
    {
        vec.append(0);
        emit W_MakeArquebusInLastGame();
    }
    if(!rpum.contains("Hauberk"))
    {
        vec.append(1);
        emit W_HauberkExploredInLastGame();
    }
    if(!rpum.contains("Armor"))
    {
        vec.append(2);
        emit W_ArmorExploredInLastGame();
    }
    if(!rpum.contains("Weapon"))
    {
        vec.append(3);
        emit W_WeaponLevelUpInLastGame();
    }

    return vec;
}
