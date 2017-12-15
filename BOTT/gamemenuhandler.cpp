#include "gamemenuhandler.h"
#include "view.h"
#include "gamemenu.h"
#include "Military/army.h"
#include "gameMenus/mainmenu.h"
#include "gameMenus/magicmenu.h"
#include "gameMenus/workshopmenu.h"
#include "gameMenus/hospitalmenu.h"
#include "gameMenus/minemenu.h"
#include "town.h"
#include <QGraphicsScene>
#include <QGraphicsTextItem>

GameMenuHandler::GameMenuHandler(View * parent) : QObject()
{
    this->parent = parent;

    setPixmap(QPixmap(":images/images/gameMenuBackground.png"));
    mFocus = new QGraphicsPixmapItem();
    mFocus->setPixmap(QPixmap(":images/images/focus.png"));
    mPrice = new QGraphicsPixmapItem();

    priceItem = new QGraphicsTextItem();
    priceItem->setFont(QFont("Old English Text MT", 19));
    priceItem->setDefaultTextColor(QColor(255, 246, 0));

    currentOpenMenu = NULL;
    currentItem = -1;

    mainMenu = new MainMenu();
    magicMenu = new MagicMenu();
    workshopMenu = new WorkshopMenu();
    hospitalMenu = new HospitalMenu();
    mineMenu = new MineMenu();
}

GameMenuHandler::~GameMenuHandler()
{
    delete mainMenu;
    delete magicMenu;
    delete workshopMenu;
    delete hospitalMenu;
    delete mineMenu;
    delete mFocus;
    delete mPrice;
    delete priceItem;
}

void GameMenuHandler::addMenusToScene()
{
    mainMenu->addMenuToScene(this);
    magicMenu->addMenuToScene(this);
    workshopMenu->addMenuToScene(this);
    hospitalMenu->addMenuToScene(this);
    mineMenu->addMenuToScene(this);

    mFocus->setVisible(false);
    mFocus->setPos((this->pos().x() + this->pixmap().width() / 2) - mFocus->pixmap().width() / 2, 0);
    scene()->addItem(mFocus);

    if(side)
    {
        mPrice->setPos(this->pos().x() - mPrice->pixmap().width(), 0);
        priceItem->setPos(this->pos().x() - mPrice->pixmap().width() + 5, 0);
    }
    else
    {
        mPrice->setPos(this->pos().x() + this->pixmap().width(), 0);
        priceItem->setPos(this->pos().x() + this->pixmap().width() + 10, 0);
    }

    mPrice->setVisible(false);
    scene()->addItem(mPrice);
    priceItem->setVisible(false);
    scene()->addItem(priceItem);
}

void GameMenuHandler::setPriceSid(bool side) // right - true, false - left
{
    this->side = side;

    if(side)
        mPrice->setPixmap(QPixmap(":images/images/priceRight.png"));
    else mPrice->setPixmap(QPixmap(":images/images/priceLeft.png"));
}

View *GameMenuHandler::getParentView()
{
    return parent;
}

void GameMenuHandler::showMainMenu()
{
    mFocus->setVisible(false);
    currentOpenMenu = mainMenu;
    currentOpenMenu->setMenuVisible(true);
}

void GameMenuHandler::showMagicMenu()
{
    mFocus->setVisible(false);
    currentOpenMenu = magicMenu;
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
    int i = 0;
    if(currentItem == -1)
    {
        if(upOrDown)
        {
            for(i = currentOpenMenu->getMenuItems()->size() - 1; currentOpenMenu->getMenuItems()->at(i) == NULL; i--);

            mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
            mPrice->setPos(mPrice->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
            currentItem = i;
        }
        else
        {
            for(i = 0; currentOpenMenu->getMenuItems()->at(i) == NULL; i++);

            mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
            mPrice->setPos(mPrice->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
            currentItem = i;
        }
    }
    else
    {
        if(upOrDown)
        {
            if(currentItem - 1 < 0)
            {
                for(i = currentOpenMenu->getMenuItems()->size() - 1; currentOpenMenu->getMenuItems()->at(i) == NULL; i--);

                mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
                mPrice->setPos(mPrice->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
                currentItem = i;
            }
            else
            {
                for(i = currentItem - 1; i >= 0 && currentOpenMenu->getMenuItems()->at(i) == NULL; i--);

                if(i == -1)
                    for(i = currentOpenMenu->getMenuItems()->size() - 1; currentOpenMenu->getMenuItems()->at(i) == NULL; i--);

                mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
                mPrice->setPos(mPrice->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
                currentItem = i;
            }
        }
        else
        {
            if(currentItem + 1 > currentOpenMenu->getMenuItems()->size() - 1)
            {
                for(i = 0; i < currentOpenMenu->getMenuItems()->size() && currentOpenMenu->getMenuItems()->at(i) == NULL; i++);

                mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
                mPrice->setPos(mPrice->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
                currentItem = i;
            }
            else
            {
                try
                {
                    for(i = currentItem + 1; i < currentOpenMenu->getMenuItems()->size() && currentOpenMenu->getMenuItems()->at(i) == NULL; i++);

                    if(currentOpenMenu->getMenuItems()->value(i) == NULL)
                        throw new QString("itemIsNULL");

                    mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
                    mPrice->setPos(mPrice->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
                    currentItem = i;
                }
                catch(QString *)
                {
                    for(i = 0; i < currentOpenMenu->getMenuItems()->size() && currentOpenMenu->getMenuItems()->at(i) == NULL; i++);

                    mFocus->setPos(mFocus->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
                    mPrice->setPos(mPrice->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
                    currentItem = i;
                }
            }
        }
    }
    mFocus->setVisible(true);
    if(currentOpenMenu != mainMenu)
    {
        mPrice->setVisible(true);
        priceItem->setPlainText(QString::number(currentOpenMenu->getPriceOfCurrentItem(parent->getPriceUpgradeMap(), currentItem)));
        if(side)
            priceItem->setPos(mPrice->pos().x() + 5, mPrice->pos().y() - 2);
        else priceItem->setPos(mPrice->pos().x() + 10, mPrice->pos().y() - 2);
        priceItem->setVisible(true);
    }
}

void GameMenuHandler::hideCurrentOpenMenu()
{
    currentOpenMenu->setMenuVisible(false);
    currentOpenMenu = NULL;
    currentItem = -1;
    mFocus->setVisible(false);
    mPrice->setVisible(false);
    priceItem->setVisible(false);
}

void GameMenuHandler::processSelectAction()
{
    currentOpenMenu->processSelectAction(currentItem);
    currentItem = -1;
    mFocus->setVisible(false);
    mPrice->setVisible(false);
    priceItem->setVisible(false);
}

void GameMenuHandler::processExitAction()
{
    currentOpenMenu->processExitAction();
}

void GameMenuHandler::deleteCurrentMenuItem()
{
    int i;

    if(currentItem != currentOpenMenu->getMenuItems()->size() - 1)
    {
        int tempY = currentOpenMenu->getMenuItems()->at(currentItem)->pos().y();

        for(i = currentItem + 1; i < currentOpenMenu->getMenuItems()->size() && currentOpenMenu->getMenuItems()->at(i) == NULL; i++);

        if(currentOpenMenu->getMenuItems()->value(i) != NULL)
        {
            tempY = currentOpenMenu->getMenuItems()->at(i)->pos().y() - tempY;
            for(i; i < currentOpenMenu->getMenuItems()->size(); i++)
                currentOpenMenu->getMenuItems()->at(i)->setY(currentOpenMenu->getMenuItems()->at(i)->pos().y() - tempY);
        }
    }

    delete currentOpenMenu->getMenuItems()->at(currentItem);

    currentOpenMenu->getMenuItems()->replace(currentItem, NULL);
    currentItem = -1;
    mFocus->setVisible(false);
    mPrice->setVisible(false);
    priceItem->setVisible(false);
    mFocus->setPos((this->pos().x() + this->pixmap().width() / 2) - mFocus->pixmap().width() / 2, 0);
    mPrice->setPos(mPrice->pos().x(), 0);
    priceItem->setPos(priceItem->pos().x(), 0);

    i = 0;

    for(int j = 0; j < currentOpenMenu->getMenuItems()->size(); j++)
        if(currentOpenMenu->getMenuItems()->at(j) == NULL)
            i++;

    if(i == currentOpenMenu->getMenuItems()->size())
    {
        scene()->removeItem(currentOpenMenu->getNameOfMenu());
        delete currentOpenMenu->getNameOfMenu();
        currentOpenMenu->setNameOfMenu(NULL);
        for(int i = 0; i < currentOpenMenu->getMenuItems()->size(); i++)
        {
            //scene()->removeItem(currentOpenMenu->getMenuItems()->at(i));
            delete currentOpenMenu->getMenuItems()->at(i);
        }

        if(currentOpenMenu == magicMenu)
            currentItem = 0;
        if(currentOpenMenu == workshopMenu)
            currentItem = 1;
        if(currentOpenMenu == hospitalMenu)
            currentItem = 2;
        if(currentOpenMenu == mineMenu)
            currentItem = 3;

        showMainMenu();
        deleteCurrentMenuItem();
    }
}

void GameMenuHandler::connectToMenus(QObject * objectForConnect)
{
    magicMenu->connectWithObject(objectForConnect);
    workshopMenu->connectWithObject(objectForConnect);
    hospitalMenu->connectWithObject(objectForConnect);
    mineMenu->connectWithObject(objectForConnect);
}
