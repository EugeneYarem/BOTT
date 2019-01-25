#include "constants.h"
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
        lastSceneX = LAST_SCENE_X_FOR_BOTTOM_VIEW;

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
    inMenuTimer_interval = IN_MENU_INTERVAL;
    pauseMenuTimer_interval = MENU_PAUSE_INTERVAL;
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

    priceUpgrade.insert("Hauberk", HAUBERK_PRICE);
    priceUpgrade.insert("Armor", ARMOR_PRICE);
    priceUpgrade.insert("Weapon", WEAPON_PRICE);
    priceUpgrade.insert("Quarantine", QUARANTINE_PRICE);
    priceUpgrade.insert("Doctors", DOCTORS_PRICE);
    priceUpgrade.insert("Mage_Hp", MAGE_HP_PRICE);
    priceUpgrade.insert("Mage_Attack", MAGE_ATTACK_PRICE);
    priceUpgrade.insert("Arquebus", ARQUEBUS_PRICE);
    priceUpgrade.insert("Mine level up", MINE_LEVEL_UP_PRICE);
}

bool View::isCanMenuOpen() const
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

void View::resizeEvent(QResizeEvent * event)
{
    // В этом if настраивается sceneRect нижнего view после события View::resizeEvent

    if(this->viewPosition == ViewPosition::BottomView)
    {
        if(event->size().width() == LAST_SCENE_X_FOR_BOTTOM_VIEW + 1)
        {
            lastSceneX = LAST_SCENE_X_FOR_BOTTOM_VIEW;
            setSceneRect(LAST_SCENE_X_FOR_BOTTOM_VIEW, 0, LAST_SCENE_X_FOR_BOTTOM_VIEW, SCENE_HEIGHT);
            return;
        }
        setSceneRect(lastSceneX - (event->size().width() - event->oldSize().width()), 0, scene()->width() - lastSceneX , SCENE_HEIGHT);
        lastSceneX = lastSceneX - (event->size().width() - event->oldSize().width());
    }
}

void View::keyPressEvent(QKeyEvent * event)
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

bool View::event(QEvent * event)
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

void View::configureControlKeys(QVector<int> * errors)
{
    if(errors->isEmpty())
    {
        delete errors;
        return;
    }

    if(this->viewPosition == ViewPosition::BottomView)
    {
        if(errors->contains(0))
            controlKeys.insert("menu", MENU_P2);
        if(errors->contains(1))
            controlKeys.insert("menu up", MENU_UP_P2);
        if(errors->contains(2))
            controlKeys.insert("menu down", MENU_DOWN_P2);
        if(errors->contains(3))
            controlKeys.insert("menu select", MENU_SELECT_P2);
        if(errors->contains(4))
            controlKeys.insert("exit from menu", EXIT_FROM_MENU_P2);
        if(errors->contains(5))
            controlKeys.insert("create soldier", CREATE_SOLDIER_P2);
        if(errors->contains(6))
            controlKeys.insert("create archer", CREATE_ARCHER_P2);
        if(errors->contains(7))
            controlKeys.insert("create rider", CREATE_RIDER_P2);
        if(errors->contains(8))
            controlKeys.insert("create wizard", CREATE_WIZARD_P2);
    }
    else
    {
        if(errors->contains(0))
            controlKeys.insert("menu", MENU_P1);
        if(errors->contains(1))
            controlKeys.insert("menu up", MENU_UP_P1);
        if(errors->contains(2))
            controlKeys.insert("menu down", MENU_DOWN_P1);
        if(errors->contains(3))
            controlKeys.insert("menu select", MENU_SELECT_P1);
        if(errors->contains(4))
            controlKeys.insert("exit from menu", EXIT_FROM_MENU_P1);
        if(errors->contains(5))
            controlKeys.insert("create soldier", CREATE_SOLDIER_P1);
        if(errors->contains(6))
            controlKeys.insert("create archer", CREATE_ARCHER_P1);
        if(errors->contains(7))
            controlKeys.insert("create rider", CREATE_RIDER_P1);
        if(errors->contains(8))
            controlKeys.insert("create wizard", CREATE_WIZARD_P1);
    }

    delete errors;
}

bool View::isControlKey(const quint32 & key) const
{
    foreach (Qt::Key value, controlKeys) {
        if(value == key)
            return true;
    }
    return false;
}

bool View::isCKContainKeyWithoutCrossingWithTS(const Qt::Key & key) const
{
    QMap<QString, Qt::Key>::const_iterator i = controlKeys.cbegin();
    while(i != controlKeys.cend())
    {
        if( !tempSettings.contains(i.key()) && i.value() == key )
            return true;
        i++;
    }
    return false;
}

bool View::isControlKey(const int & key) const
{
    foreach (Qt::Key value, controlKeys) {
        if(value == key)
            return true;
    }
    return false;
}

bool View::isShortcut(const quint32 & key) const
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

bool View::isShortcut(const int & key) const
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

Qt::Key View::getControlKey(const QString & key) const
{
    return controlKeys[key];
}

QString View::getValueByControlKey(const QString & key, const SettingMap & map) const
{
    foreach (Qt::Key value, map == SettingMap::Main ? controlKeys : tempSettings) {
        if(QKeySequence(value).toString() == key)
            return map == SettingMap::Main ? controlKeys.key(value) : tempSettings.key(value);
    }
    return "";
}

void View::setControlKey(const QString & key, const Qt::Key & value)
{
    controlKeys[key] = value;
}

bool View::isControlKey(const Qt::Key & key, const SettingMap & map) const
{
    foreach (Qt::Key value, map == SettingMap::Main ? controlKeys : tempSettings) {
        if(value == key)
            return true;
    }
    return false;
}

void View::setPriceUpgrade(const QString & key, const int & value)
{
    //priceUpgrade[key] = value;
    priceUpgrade.insert(key, value);
}

void View::setTempControlKey(const QString & keyInControlMap, const Qt::Key & keyValue)
{
    tempSettings.insert(keyInControlMap, keyValue);
}

int View::getPriceUpgrade(const QString & key) const
{
    return priceUpgrade[key];
}

Army * View::getArmy()
{
    return army;
}

Town * View::getTown() const
{
    return town;
}

void View::clearPriceUpgradeMap()
{
    priceUpgrade.clear();
}

void View::clearTempSettings()
{
    tempSettings.clear();
}

void View::stopAllTimers()
{
    inMenuTimer_remainingTime = inMenuTimer->remainingTime();
    pauseMenuTimer_remainingTime = pauseMenuTimer->remainingTime();
    inMenuTimer->stop();
    pauseMenuTimer->stop();

    town->stopAllTimers();
}

void View::startAllTimers() const
{
    inMenuTimer->start(inMenuTimer_remainingTime);
    pauseMenuTimer->start(pauseMenuTimer_remainingTime);
    town->startAllTimers();
}

void View::deleteCurrentMenuItem() const
{
    gameMenu->deleteCurrentMenuItem();
}

void View::modificateFromLastGame()
{
    gameMenu->restoreLastGame(priceUpgrade);
}

void View::removePriceUpgrade(const QString & keyInPriceMap)
{
    priceUpgrade.remove(keyInPriceMap);
}

void View::replaceCKByTS()
{
    if(!tempSettings.isEmpty())
    {
        QMap<QString, Qt::Key>::const_iterator i = tempSettings.cbegin();
        while(i != tempSettings.cend())
        {
            controlKeys[i.key()] = i.value();
            i++;
        }
        tempSettings.clear();
    }
}

QMap<QString, Qt::Key> & View::getControlKeys()
{
    return controlKeys;
}

const QMap<QString, int> * View::getPriceUpgradeMap() const
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

GameMenuHandler * View::getGameMenu() const
{
    return gameMenu;
}
