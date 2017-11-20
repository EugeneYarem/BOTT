#include "hospitalmenu.h"
#include "gamemenuhandler.h"
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
    if(currentItem == 0)
        emit H_EnterQuarantine();
    if(currentItem == 1)
        emit H_DoctorsCountUp();
}

void HospitalMenu::connectWithBuild(QGraphicsPixmapItem * build)
{
    //connect(this, SIGNAL(H_EnterQuarantine()), build, SLOT());
    //connect(this, SIGNAL(H_DoctorsCountUp()), build, SLOT());
}
