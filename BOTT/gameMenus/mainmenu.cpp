#include "mainmenu.h"
#include "gamemenuhandler.h"
#include "view.h"
#include "town.h"
#include "message.h"
#include "Military/army.h"
#include <QGraphicsScene>

MainMenu::MainMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    nameOfMenu->setPixmap(QPixmap(":images/images/Main_Menu/mMain.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem(),
                        * m2 = new QGraphicsPixmapItem(),
                        * m3 = new QGraphicsPixmapItem(),
                        * m4 = new QGraphicsPixmapItem(),
                        * m5 = new QGraphicsPixmapItem();

    m1->setPixmap(QPixmap(":images/images/Main_Menu/mSc.png"));
    m2->setPixmap(QPixmap(":images/images/Main_Menu/mW.png"));
    m3->setPixmap(QPixmap(":images/images/Main_Menu/mH.png"));
    m4->setPixmap(QPixmap(":images/images/Main_Menu/mM.png"));
    m5->setPixmap(QPixmap(":images/images/Main_Menu/mC.png"));
    menuItems.push_back(m1);
    menuItems.push_back(m2);
    menuItems.push_back(m3);
    menuItems.push_back(m4);
    menuItems.push_back(m5);
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
    if(currentItem == 4)
    {
        parent->showMainMenu();
        Message message(parent->getParentView());
        connect(&message, SIGNAL(okButtonPress()), parent->getParentView()->getTown(), SIGNAL(loose()));
        message.setMessage("Вы точно желаете сдаться?");
        message.show();
        message.exec();
    }
}

void MainMenu::processExitAction()
{
    emit parent->closeMenu();
}

void MainMenu::connectWithObject(QObject * objectForConnect)
{}

void MainMenu::deleteMenuItem(int item)
{
    parent->scene()->removeItem(menuItems.at(item));
    delete menuItems.at(item);
}

int MainMenu::getPriceOfCurrentItem(QMap<QString, int> *, int)
{}
