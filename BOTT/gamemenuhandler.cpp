#include "gamemenu.h"
#include "gamemenuhandler.h"
#include "gameMenus/hospitalmenu.h"
#include "gameMenus/magicmenu.h"
#include "gameMenus/mainmenu.h"
#include "gameMenus/minemenu.h"
#include "gameMenus/workshopmenu.h"
#include "view.h"


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

    currentOpenMenu = nullptr;
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

    if(this->side == ConflictSide::Right)
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

void GameMenuHandler::setPriceSide(const ConflictSide & side)
{
    this->side = side;

    if(this->side == ConflictSide::Right)
        mPrice->setPixmap(QPixmap(":images/images/priceRight.png"));
    else mPrice->setPixmap(QPixmap(":images/images/priceLeft.png"));
}

View * GameMenuHandler::getParentView() const
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

void GameMenuHandler::setCurrentItem(const DirectionInMenu & upOrDown) // true - вверх, false - вниз
{
    int i = 0;
    if(currentItem == -1)
    {
        if(upOrDown == DirectionInMenu::Up)
            for(i = currentOpenMenu->getMenuItems()->size() - 1; currentOpenMenu->getMenuItems()->at(i) == nullptr; i--);
        else
            for(i = 0; currentOpenMenu->getMenuItems()->at(i) == nullptr; i++);
    }
    else
    {
        if(upOrDown == DirectionInMenu::Up)
        {
            if(currentItem - 1 < 0)
                for(i = currentOpenMenu->getMenuItems()->size() - 1; currentOpenMenu->getMenuItems()->at(i) == nullptr; i--);
            else
            {
                for(i = currentItem - 1; i >= 0 && currentOpenMenu->getMenuItems()->at(i) == nullptr; i--);
                if(i == -1)
                    for(i = currentOpenMenu->getMenuItems()->size() - 1; currentOpenMenu->getMenuItems()->at(i) == nullptr; i--);
            }
        }
        else
        {
            if(currentItem + 1 > currentOpenMenu->getMenuItems()->size() - 1)
                for(i = 0; i < currentOpenMenu->getMenuItems()->size() && currentOpenMenu->getMenuItems()->at(i) == nullptr; i++);
            else
            {
                try
                {
                    for(i = currentItem + 1; i < currentOpenMenu->getMenuItems()->size() && currentOpenMenu->getMenuItems()->at(i) == nullptr; i++);

                    if(currentOpenMenu->getMenuItems()->value(i) == nullptr)
                        throw new QString("itemIsNULL");
                }
                catch(QString *)
                {
                    for(i = 0; i < currentOpenMenu->getMenuItems()->size() && currentOpenMenu->getMenuItems()->at(i) == nullptr; i++);
                }
            }
        }
    }

    setFocusAndPricePos(mFocus->pos().x(), mPrice->pos().x(), currentOpenMenu->getMenuItems()->value(i)->pos().y());
    currentItem = i;

    mFocus->setVisible(true);
    if(currentOpenMenu != mainMenu)
    {
        mPrice->setVisible(true);
        priceItem->setPlainText(QString::number(currentOpenMenu->getPriceOfCurrentItem(parent->getPriceUpgradeMap(), currentItem)));
        if(this->side == ConflictSide::Right)
            priceItem->setPos(mPrice->pos().x() + 5, mPrice->pos().y() - 2);
        else priceItem->setPos(mPrice->pos().x() + 10, mPrice->pos().y() - 2);
        priceItem->setVisible(true);
    }
}

void GameMenuHandler::hideCurrentOpenMenu()
{
    currentOpenMenu->setMenuVisible(false);
    currentOpenMenu = nullptr;
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

void GameMenuHandler::restoreLastGame(const QMap<QString, int> & rpum)
{
    currentOpenMenu = hospitalMenu;
    foreach(int i, hospitalMenu->restoreLastGame(rpum))
    {
        currentItem = i;
        deleteCurrentMenuItem();
    }
    currentOpenMenu = magicMenu;
    foreach(int i, magicMenu->restoreLastGame(rpum))
    {
        currentItem = i;
        deleteCurrentMenuItem();
    }
    currentOpenMenu = mineMenu;
    foreach(int i, mineMenu->restoreLastGame(rpum))
    {
        currentItem = i;
        deleteCurrentMenuItem();
    }
    currentOpenMenu = workshopMenu;
    foreach(int i, workshopMenu->restoreLastGame(rpum))
    {
        currentItem = i;
        deleteCurrentMenuItem();
    }
    hideCurrentOpenMenu();
}

void GameMenuHandler::processExitAction() const
{
    currentOpenMenu->processExitAction();
}

void GameMenuHandler::deleteCurrentMenuItem()
{
    int i;

    if(currentItem != currentOpenMenu->getMenuItems()->size() - 1)
    {
        long int tempY = static_cast<long int>(currentOpenMenu->getMenuItems()->at(currentItem)->pos().y());

        for(i = currentItem + 1; i < currentOpenMenu->getMenuItems()->size() && currentOpenMenu->getMenuItems()->at(i) == nullptr; i++);

        if(currentOpenMenu->getMenuItems()->value(i) != nullptr)
        {
            tempY = static_cast<long int>(currentOpenMenu->getMenuItems()->at(i)->pos().y() - tempY);
            for( ; i < currentOpenMenu->getMenuItems()->size(); i++)
                currentOpenMenu->getMenuItems()->at(i)->setY(currentOpenMenu->getMenuItems()->at(i)->pos().y() - tempY);
        }
    }

    delete currentOpenMenu->getMenuItems()->at(currentItem);

    currentOpenMenu->getMenuItems()->replace(currentItem, nullptr);
    currentItem = -1;
    mFocus->setVisible(false);
    mPrice->setVisible(false);
    priceItem->setVisible(false);
    mFocus->setPos((this->pos().x() + this->pixmap().width() / 2) - mFocus->pixmap().width() / 2, 0);
    mPrice->setPos(mPrice->pos().x(), 0);
    priceItem->setPos(priceItem->pos().x(), 0);

    i = 0;

    for(int j = 0; j < currentOpenMenu->getMenuItems()->size(); j++)
        if(currentOpenMenu->getMenuItems()->at(j) == nullptr)
            i++;

    if(i == currentOpenMenu->getMenuItems()->size())
    {
        scene()->removeItem(currentOpenMenu->getNameOfMenu());
        delete currentOpenMenu->getNameOfMenu();
        currentOpenMenu->setNameOfMenu(nullptr);
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

void GameMenuHandler::connectToMenus(const QObject * objectForConnect) const
{
    magicMenu->connectWithObject(objectForConnect);
    workshopMenu->connectWithObject(objectForConnect);
    hospitalMenu->connectWithObject(objectForConnect);
    mineMenu->connectWithObject(objectForConnect);
    mainMenu->connectWithObject(objectForConnect);
}

void GameMenuHandler::setFocusAndPricePos(const qreal & xF, const qreal & xP, const qreal & y) const
{
    mFocus->setPos(xF, y);
    mPrice->setPos(xP, y);
}
