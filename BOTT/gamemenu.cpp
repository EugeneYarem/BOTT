#include "gamemenu.h"
#include "gamemenuhandler.h"
#include <QGraphicsScene>
#include <QDebug>

GameMenu::GameMenu() : QObject()
{}

GameMenu::~GameMenu()
{
    if(nameOfMenu != NULL)
        delete nameOfMenu;
}

void GameMenu::addMenuToScene(GameMenuHandler * parent)
{
    this->parent = parent;

    nameOfMenu->setPos((parent->pos().x() + parent->pixmap().width() / 2) - nameOfMenu->pixmap().width() / 2, 16);
    nameOfMenu->setVisible(false);
    parent->scene()->addItem(nameOfMenu);

    int currentPosX = (parent->pos().x() + parent->pixmap().width() / 2) - menuItems[0]->pixmap().width() / 2,
        currentPosY = 70;

    for(int i = 0; i < menuItems.size(); i++)
    {
        menuItems[i]->setPos(currentPosX, currentPosY);
        menuItems[i]->setVisible(false);
        parent->scene()->addItem(menuItems[i]);
        currentPosY += menuItems[i]->pixmap().height() + 10;
    }
}

void GameMenu::setMenuVisible(bool visible)
{
    nameOfMenu->setVisible(visible);

    for(int i = 0; i < menuItems.size(); i++)
    {
        if(menuItems[i] != NULL)
            menuItems[i]->setVisible(visible);
    }
}

QVector<QGraphicsPixmapItem *> * GameMenu::getMenuItems()
{
    return &menuItems;
}

QGraphicsPixmapItem *GameMenu::getNameOfMenu()
{
    return nameOfMenu;
}

void GameMenu::setNameOfMenu(QGraphicsPixmapItem * item)
{
    nameOfMenu = item;
}

void GameMenu::processExitAction()
{
    parent->hideCurrentOpenMenu();
    parent->showMainMenu();
}
