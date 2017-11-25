#include "gamemenuhandler.h"
#include "gamemenu.h"
#include "Military/army.h"
#include "gameMenus/mainmenu.h"
#include "gameMenus/sciencemenu.h"
#include "gameMenus/workshopmenu.h"
#include "gameMenus/hospitalmenu.h"
#include "gameMenus/minemenu.h"
#include "town.h"
#include <QGraphicsScene>
#include <QDebug>

GameMenuHandler::GameMenuHandler() : QObject()
{
    setPixmap(QPixmap(":images/images/gameMenuBackground.png"));
    mFocus = new QGraphicsPixmapItem();
    mFocus->setPixmap(QPixmap(":images/images/focus.png"));
    currentOpenMenu = NULL;
    currentItem = -1;

    mainMenu = new MainMenu();
    scienceMenu = new ScienceMenu();
    workshopMenu = new WorkshopMenu();
    hospitalMenu = new HospitalMenu();
    mineMenu = new MineMenu();
}

GameMenuHandler::~GameMenuHandler()
{
    delete mainMenu;
    delete scienceMenu;
    delete workshopMenu;
    delete hospitalMenu;
    delete mineMenu;
    delete mFocus;
}

void GameMenuHandler::addMenusToScene()
{
    mainMenu->addMenuToScene(this);
    scienceMenu->addMenuToScene(this);
    workshopMenu->addMenuToScene(this);
    hospitalMenu->addMenuToScene(this);
    mineMenu->addMenuToScene(this);

    mFocus->setVisible(false);
    mFocus->setPos((this->pos().x() + this->pixmap().width() / 2) - mFocus->pixmap().width() / 2, 0);
    scene()->addItem(mFocus);
}

void GameMenuHandler::showMainMenu()
{
    mFocus->setVisible(false);
    currentOpenMenu = mainMenu;
    currentOpenMenu->setMenuVisible(true);
}

void GameMenuHandler::showScienceMenu()
{
    mFocus->setVisible(false);
    currentOpenMenu = scienceMenu;
    currentOpenMenu->setMenuVisible(true);
}

void GameMenuHandler::showWorkshopMenu()
{
    mFocus->setVisible(false);
    currentOpenMenu = workshopMenu;
    currentOpenMenu->setMenuVisible(true);
}

void GameMenuHandler::showHospitalMenu()
{
    mFocus->setVisible(false);
    currentOpenMenu = hospitalMenu;
    currentOpenMenu->setMenuVisible(true);
}

void GameMenuHandler::showMineMenu()
{
    mFocus->setVisible(false);
    currentOpenMenu = mineMenu;
    currentOpenMenu->setMenuVisible(true);
}

void GameMenuHandler::setCurrentItem(bool upOrDown) // true - вверх, false - вниз
{
    if(currentItem == -1)
    {
        if(upOrDown)
        {
            mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(currentOpenMenu->getMenuItems()->size() - 1)->pos().y());
            currentItem = currentOpenMenu->getMenuItems()->size() - 1;
        }
        else
        {
            mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(0)->pos().y());
            currentItem = 0;
        }
    }
    else
    {
        if(upOrDown)
        {
            if(currentItem - 1 < 0)
            {
                mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(currentOpenMenu->getMenuItems()->size() - 1)->pos().y());
                currentItem = currentOpenMenu->getMenuItems()->size() - 1;
            }
            else
            {
                mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(currentItem - 1)->pos().y());
                currentItem--;
            }
        }
        else
        {
            if(currentItem + 1 > currentOpenMenu->getMenuItems()->size() - 1)
            {
                mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(0)->pos().y());
                currentItem = 0;
            }
            else
            {
                mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(currentItem + 1)->pos().y());
                currentItem++;
            }
        }
    }

    mFocus->setVisible(true);
}

void GameMenuHandler::hideCurrentOpenMenu()
{
    currentOpenMenu->setMenuVisible(false);
    currentOpenMenu = NULL;
    currentItem = -1;
    mFocus->setVisible(false);
}

void GameMenuHandler::processSelectAction()
{
    currentOpenMenu->processSelectAction(currentItem);
}

void GameMenuHandler::processExitAction()
{
    currentOpenMenu->processExitAction();
}

void GameMenuHandler::connectToMenus(QObject * objectForConnect)
{
    scienceMenu->connectWithObject(objectForConnect);
    workshopMenu->connectWithObject(objectForConnect);
    hospitalMenu->connectWithObject(objectForConnect);
    mineMenu->connectWithObject(objectForConnect);
}
