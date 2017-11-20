#include "mainmenu.h"
#include "gamemenuhandler.h"
#include <QDebug>

MainMenu::MainMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    //nameOfMenu->setPixmap(QPixmap(":images/images/mHospital.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem(),
                        * m2 = new QGraphicsPixmapItem(),
                        * m3 = new QGraphicsPixmapItem(),
                        * m4 = new QGraphicsPixmapItem(),
                        * m5 = new QGraphicsPixmapItem();

    m1->setPixmap(QPixmap(":images/images/mSc.png"));
    m2->setPixmap(QPixmap(":images/images/mW.png"));
    m3->setPixmap(QPixmap(":images/images/mH.png"));
    m4->setPixmap(QPixmap(":images/images/mM.png"));
    m5->setPixmap(QPixmap(":images/images/mB.png"));
    menuItems.push_back(m1);
    menuItems.push_back(m2);
    menuItems.push_back(m3);
    menuItems.push_back(m4);
    menuItems.push_back(m5);
}

void MainMenu::processSelectAction(int currentItem)
{
    parent->hideCurrentOpenMenu();

    if(currentItem == 0)
        parent->showScienceMenu();
    if(currentItem == 1)
        parent->showWorkshopMenu();
    if(currentItem == 2)
        parent->showHospitalMenu();
    if(currentItem == 3)
        parent->showMineMenu();
}

void MainMenu::processExitAction()
{
    emit parent->closeMenu();
}

void MainMenu::connectWithBuild(QGraphicsPixmapItem *)
{}
