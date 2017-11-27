#include "mainmenu.h"
#include "gamemenuhandler.h"
#include "Military/army.h"
#include <QDebug>

MainMenu::MainMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    nameOfMenu->setPixmap(QPixmap(":images/images/Main_Menu/mMain.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem(),
                        * m2 = new QGraphicsPixmapItem(),
                        * m3 = new QGraphicsPixmapItem(),
                        * m4 = new QGraphicsPixmapItem();

    m1->setPixmap(QPixmap(":images/images/Main_Menu/mSc.png"));
    m2->setPixmap(QPixmap(":images/images/Main_Menu/mW.png"));
    m3->setPixmap(QPixmap(":images/images/Main_Menu/mH.png"));
    m4->setPixmap(QPixmap(":images/images/Main_Menu/mM.png"));
    menuItems.push_back(m1);
    menuItems.push_back(m2);
    menuItems.push_back(m3);
    menuItems.push_back(m4);
}

void MainMenu::processSelectAction(int currentItem)
{
    if(currentItem == -1)
        return;

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

void MainMenu::connectWithObject(QObject * objectForConnect)
{}
