#include "hospitalmenu.h"
#include "gamemenuhandler.h"
#include "Military/army.h"
#include <QDebug>

HospitalMenu::HospitalMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    nameOfMenu->setPixmap(QPixmap(":images/images/Hospital_Menu/mHospital.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem(),
                        * m2 = new QGraphicsPixmapItem();

    m1->setPixmap(QPixmap(":images/images/Hospital_Menu/h1.png"));
    m2->setPixmap(QPixmap(":images/images/Hospital_Menu/h2.png"));
    menuItems.push_back(m1);
    menuItems.push_back(m2);
}

void HospitalMenu::processSelectAction(int currentItem)
{
    if(currentItem == -1)
        return;
    if(currentItem == 0)
        emit H_EnterQuarantine();
    if(currentItem == 1)
        emit H_DoctorsCountUp();
}

void HospitalMenu::connectWithObject(QObject * objectForConnect)
{
    if(typeid(*objectForConnect) == typeid(Army))
    {
        //connect(this, SIGNAL(H_EnterQuarantine()), (Army *)objectForConnect, SLOT());
        //connect(this, SIGNAL(H_DoctorsCountUp()), (Army *)objectForConnect, SLOT());
    }
}
