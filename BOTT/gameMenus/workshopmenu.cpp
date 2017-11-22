#include "workshopmenu.h"
#include "gamemenuhandler.h"
#include "Military/army.h"
#include <QDebug>

WorkshopMenu::WorkshopMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    nameOfMenu->setPixmap(QPixmap(":images/images/Workshop_Menu/mWorkshop.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem(),
                        * m2 = new QGraphicsPixmapItem(),
                        * m3 = new QGraphicsPixmapItem(),
                        * m4 = new QGraphicsPixmapItem(),
                        * m5 = new QGraphicsPixmapItem();

    m1->setPixmap(QPixmap(":images/images/Workshop_Menu/w1.png"));
    m2->setPixmap(QPixmap(":images/images/Workshop_Menu/w2.png"));
    m3->setPixmap(QPixmap(":images/images/Workshop_Menu/w3.png"));
    m4->setPixmap(QPixmap(":images/images/Workshop_Menu/w4.png"));
    m5->setPixmap(QPixmap(":images/images/Workshop_Menu/w5.png"));
    menuItems.push_back(m1);
    menuItems.push_back(m2);
    menuItems.push_back(m3);
    menuItems.push_back(m4);
    menuItems.push_back(m5);
}

void WorkshopMenu::processSelectAction(int currentItem)
{
    if(currentItem == -1)
        return;
    if(currentItem == 0)
        emit W_MakeGun();
    if(currentItem == 1)
        emit W_MakeArquebus();
    if(currentItem == 2)
        emit W_HauberkExplored();
    if(currentItem == 3)
        emit W_ArmorExplored();
    if(currentItem == 4)
        emit W_WeaponLevelUp();
}

void WorkshopMenu::connectWithObject(QObject * objectForConnect)
{
    if(typeid(*objectForConnect) == typeid(Army))
    {
        /*connect(this, SIGNAL(W_MakeGun()), build, SLOT());
        connect(this, SIGNAL(W_MakeArquebus()), build, SLOT());
        connect(this, SIGNAL(W_HauberkExplored()), build, SLOT());
        connect(this, SIGNAL(W_ArmorExplored()), build, SLOT());
        connect(this, SIGNAL(W_WeaponLevelUp()), build, SLOT());*/
    }
}
