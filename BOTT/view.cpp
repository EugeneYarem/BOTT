#include "enums.h"
#include "gamemenuhandler.h"
#include "Military/army.h"
#include "town.h"
#include "view.h"
#include "widget.h"
#include <QResizeEvent>
#include <QTimer>


View::View(ViewPosition viewPosition, Widget * parent) : QGraphicsView(parent)
{
    this->mainParent = parent;
    this->viewPosition = viewPosition;

    if(this->viewPosition == ViewPosition::BottomView)
        lastSceneX = 1255;

    gameMenu = new GameMenuHandler(this);
    connect(gameMenu, &GameMenuHandler::closeMenu, this, &View::hideMenu);
    town = new Town(this);

    if(this->viewPosition == ViewPosition::BottomView)
        town->setPixmap(QPixmap(":images/images/towns/town_2.png"));
    else town->setPixmap(QPixmap(":images/images/towns/town.png"));

    if(this->viewPosition == ViewPosition::BottomView)
        army = new Army(this, ConflictSide::Right);
    else
        army = new Army(this, ConflictSide::Left);

    gameMenu->connectToMenus(town);
    gameMenu->connectToMenus(army);

    if(this->viewPosition == ViewPosition::BottomView)
        gameMenu->setPriceSide(ConflictSide::Right);
    else gameMenu->setPriceSide(ConflictSide::Left);

    canMenuOpen = true;
    menuOpen = false;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    inMenuTimer = new QTimer();
    pauseMenuTimer = new QTimer();
    inMenuTimer_interval = 20000;
    pauseMenuTimer_interval = 15000;
    inMenuTimer_remainingTime = -1;
    pauseMenuTimer_remainingTime = -1;

    connect(inMenuTimer, &QTimer::timeout, this, &View::hideMenu);
    connect(pauseMenuTimer, &QTimer::timeout, [this] () {
                    this->canMenuOpen = true;
                    this->pauseMenuTimer->stop();
    });
}

View::~View()
{
    delete gameMenu;
    delete town;
    delete army;
    delete inMenuTimer;
    delete pauseMenuTimer;
}

void View::setConfiguration()
{
    if(this->viewPosition == ViewPosition::BottomView)
    {
        gameMenu->setPos(scene()->width() - gameMenu->pixmap().width(), 0);
        town->setPos(scene()->width() - town->pixmap().width(), 0);
    }
    else
    {
        gameMenu->setPos(0, 0);
        town->setPos(0, 0);
    }

    gameMenu->setFlags(QGraphicsItem::ItemIsFocusable);
    gameMenu->setVisible(false);

    if(this->scene() != town->scene())
    {
        scene()->addItem(town);
        town->addHealthMoneyToScene();
    }

    scene()->addItem(gameMenu);
    gameMenu->addMenusToScene();

    priceUpgrade.insert("Hauberk", 5000);
    priceUpgrade.insert("Armor", 8000);
    priceUpgrade.insert("Weapon", 12000);
    priceUpgrade.insert("Quarantine", 500);
    priceUpgrade.insert("Doctors", 5000);
    priceUpgrade.insert("Mage_Hp", 2000);
    priceUpgrade.insert("Mage_Attack", 2000);
    priceUpgrade.insert("Arquebus", 5000);
    priceUpgrade.insert("Mine level up", 6000);
}

bool View::isCanMenuOpen()
{
    return canMenuOpen;
}

void View::hideMenu()
{
    // прячем меню и переводим нужные переменные в начальные значения

    gameMenu->setVisible(false);
    gameMenu->hideCurrentOpenMenu();

    canMenuOpen = false;
    menuOpen = false;

    emit menuVisibleStatusChanged(this);

    inMenuTimer->stop();
    pauseMenuTimer->start( pauseMenuTimer_interval );
}

void View::resizeEvent(QResizeEvent *event)
{
    // В этом if настраивается sceneRect нижнего view после события View::resizeEvent

    if(this->viewPosition == ViewPosition::BottomView)
    {
        if(event->size().width() == 1256)
        {
            lastSceneX = 1255;
            setSceneRect(1255, 0, 1255, 343);
            return;
        }
        setSceneRect(lastSceneX - (event->size().width() - event->oldSize().width()), 0, scene()->width() - lastSceneX , 343);
        lastSceneX = lastSceneX - (event->size().width() - event->oldSize().width());
    }
}

void View::keyPressEvent(QKeyEvent *event)
{
    if(canMenuOpen && (event->nativeVirtualKey() == getControlKey("menu") || event->key() == getControlKey("menu")))
    {
        if(menuOpen)
            return;

        if(gameMenu->scene() != this->scene())
            scene()->addItem(gameMenu);

        gameMenu->setVisible(true);
        gameMenu->setFocus();
        gameMenu->showMainMenu();

        inMenuTimer->start( inMenuTimer_interval );
        menuOpen = true;
        return;
    }
    if(menuOpen)
    {
        if(event->nativeVirtualKey() == getControlKey("menu up") || event->key() == getControlKey("menu up"))
            gameMenu->setCurrentItem( DirectionInMenu::Up );
        if(event->nativeVirtualKey() == getControlKey("menu down") || event->key() == getControlKey("menu down"))
            gameMenu->setCurrentItem( DirectionInMenu::Down );
        if(event->nativeVirtualKey() == getControlKey("menu select") || event->key() == getControlKey("menu select"))
            gameMenu->processSelectAction();
        if(event->nativeVirtualKey() == getControlKey("exit from menu") || event->key() == getControlKey("exit from menu"))
            gameMenu->processExitAction();
    }

    // В следющих 4 ифах происходит создание юнита в зависимости от нажатой кнопки. В этих ифах нужно добавить и проверку на наличие нужной суммы
    // Если есть нужная сумма, то создвать юнита и обязательно кидать сигнал moneyWasted с ценой создания этого юнита
    if(event->nativeVirtualKey() == getControlKey("create soldier") || event->key() == getControlKey("create soldier"))
        army->addTroop("soldier", scene());
    if(event->nativeVirtualKey() == getControlKey("create archer") || event->key() == getControlKey("create archer"))
        army->addTroop("archer", scene());
    if(event->nativeVirtualKey() == getControlKey("create rider") || event->key() == getControlKey("create rider"))
        army->addTroop("rider", scene());
    if(event->nativeVirtualKey() == getControlKey("create wizard") || event->key() == getControlKey("create wizard"))
        army->addTroop("mage", scene());
}

bool View::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        if((dynamic_cast<QKeyEvent *>(event))->key() == Qt::Key_Escape)
            return false;

        // Проверка, что нажатая клавиша - клавиша управления игрока

        if(isControlKey((dynamic_cast<QKeyEvent *>(event))->nativeVirtualKey()) || isControlKey((dynamic_cast<QKeyEvent *>(event))->key()))
            keyPressEvent(dynamic_cast<QKeyEvent *>(event));
        return true;
    }
    return QGraphicsView::event(event);
}

void View::configureControlKeys(QVector<int> *errors)
{
    if(errors->isEmpty())
    {
        delete errors;
        return;
    }

    if(this->viewPosition == ViewPosition::BottomView)
    {
        if(errors->contains(0))
            controlKeys.insert("menu", Qt::Key_M);
        if(errors->contains(1))
            controlKeys.insert("menu up", Qt::Key_Up);
        if(errors->contains(2))
            controlKeys.insert("menu down", Qt::Key_Down);
        if(errors->contains(3))
            controlKeys.insert("menu select", Qt::Key_Return);
        if(errors->contains(4))
            controlKeys.insert("exit from menu", Qt::Key_Backspace);
        if(errors->contains(5))
            controlKeys.insert("create soldier", Qt::Key_7);
        if(errors->contains(6))
            controlKeys.insert("create archer", Qt::Key_8);
        if(errors->contains(7))
            controlKeys.insert("create rider", Qt::Key_9);
        if(errors->contains(8))
            controlKeys.insert("create wizard", Qt::Key_0);
    }
    else
    {
        if(errors->contains(0))
            controlKeys.insert("menu", Qt::Key_Q);
        if(errors->contains(1))
            controlKeys.insert("menu up", Qt::Key_W);
        if(errors->contains(2))
            controlKeys.insert("menu down", Qt::Key_S);
        if(errors->contains(3))
            controlKeys.insert("menu select", Qt::Key_F);
        if(errors->contains(4))
            controlKeys.insert("exit from menu", Qt::Key_E);
        if(errors->contains(5))
            controlKeys.insert("create soldier", Qt::Key_1);
        if(errors->contains(6))
            controlKeys.insert("create archer", Qt::Key_2);
        if(errors->contains(7))
            controlKeys.insert("create rider", Qt::Key_3);
        if(errors->contains(8))
            controlKeys.insert("create wizard", Qt::Key_4);
    }

    delete errors;
}

bool View::isControlKey(quint32 key)
{
    foreach (Qt::Key value, controlKeys) {
        if(value == key)
            return true;
    }
    return false;
}

bool View::isControlKey(int key)
{
    foreach (Qt::Key value, controlKeys) {
        if(value == key)
            return true;
    }
    return false;
}

bool View::isShortcut(quint32 key)
{
    if(controlKeys.value("create soldier") == key)
        return true;
    if(controlKeys.value("create archer") == key)
        return true;
    if(controlKeys.value("create rider") == key)
        return true;
    if(controlKeys.value("create wizard") == key)
        return true;

    return false;
}

bool View::isShortcut(int key)
{
    if(controlKeys.value("create soldier") == key)
        return true;
    if(controlKeys.value("create archer") == key)
        return true;
    if(controlKeys.value("create rider") == key)
        return true;
    if(controlKeys.value("create wizard") == key)
        return true;

    return false;
}

Qt::Key View::getControlKey(QString key)
{
    return controlKeys[key];
}

QString View::getValueByControlKey(QString key)
{
    foreach (Qt::Key value, controlKeys) {
        if(QKeySequence(value).toString() == key)
            return controlKeys.key(value);
    }
}

void View::setControlKey(QString key, Qt::Key value)
{
    controlKeys[key] = value;
}

bool View::checkControlKey(Qt::Key key)
{
    foreach (Qt::Key value, controlKeys) {
        if(value == key)
            return true;
    }
    return false;
}

void View::setPriceUpgrade(QString key, int value)
{
    priceUpgrade[key] = value;
}

int View::getPriceUpgrade(QString key)
{
    return priceUpgrade[key];
}

Army *View::getArmy()
{
    return army;
}

Town *View::getTown()
{
    return town;
}

void View::stopAllTimers()
{
    inMenuTimer_remainingTime = inMenuTimer->remainingTime();
    pauseMenuTimer_remainingTime = pauseMenuTimer->remainingTime();
    inMenuTimer->stop();
    pauseMenuTimer->stop();

    town->stopAllTimers();
}

void View::startAllTimers()
{
    inMenuTimer->start(inMenuTimer_remainingTime);
    pauseMenuTimer->start(pauseMenuTimer_remainingTime);
    town->startAllTimers();
}

void View::deleteCurrentMenuItem()
{
    gameMenu->deleteCurrentMenuItem();
}

QMap<QString, Qt::Key> & View::getControlKeys()
{
    return controlKeys;
}

QMap<QString, int> * View::getPriceUpgradeMap()
{
    return &priceUpgrade;
}

void View::clearStart()
{
    inMenuTimer_remainingTime = -1;
    pauseMenuTimer_remainingTime = -1;
    inMenuTimer->stop();
    pauseMenuTimer->stop();
    town->clearStart();

    delete gameMenu;
    gameMenu = new GameMenuHandler(this);
    connect(gameMenu, &GameMenuHandler::closeMenu, this, &View::hideMenu);
    gameMenu->connectToMenus(town);
    gameMenu->connectToMenus(army);
    gameMenu->connectToMenus(this->mainParent);
    if(this->viewPosition == ViewPosition::BottomView)
        gameMenu->setPriceSide(ConflictSide::Right);
    else gameMenu->setPriceSide(ConflictSide::Left);

    setConfiguration();
    canMenuOpen = true;
    menuOpen = false;
}

GameMenuHandler *View::getGameMenu()
{
    return gameMenu;
}
