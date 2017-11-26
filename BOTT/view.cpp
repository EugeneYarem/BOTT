#include "view.h"
#include "gamemenuhandler.h"
#include "town.h"
#include "Military/army.h"
#include <QResizeEvent>
#include <QDebug>
#include <QTimer>

View::View(bool bottomView, QWidget * parent) : QGraphicsView(parent)
{
    this->bottomView = bottomView;

    if(bottomView)
        lastSceneX = 1255;

    gameMenu = new GameMenuHandler();
    connect(gameMenu, SIGNAL(closeMenu()), this, SLOT(hideMenu()));
    town = new Town();

    if(bottomView)
        town->setPixmap(QPixmap(":images/images/towns/town_2.png"));
    else town->setPixmap(QPixmap(":images/images/towns/town.png"));



    if(bottomView)
    {
         army = new Army(this,Right);
    }
    else
    {
          army = new Army(this,Left);
    }

    gameMenu->connectToMenus(town);
    gameMenu->connectToMenus(army);

    canMenuOpen = true;
    menuOpen = false;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    inMenuTimer = new QTimer();
    pauseMenuTimer = new QTimer();
    inMenuTimer_remainingTime = -1;
    pauseMenuTimer_remainingTime = -1;

    connect(inMenuTimer, SIGNAL(timeout()), this, SLOT(hideMenu()));
    connect(pauseMenuTimer, SIGNAL(timeout()), this, SLOT(setCanMenuOpenInTrue()));
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
    if(bottomView)
    {
        controlKeys.insert("menu", Qt::Key_M);
        controlKeys.insert("menu up", Qt::Key_Up);
        controlKeys.insert("menu down", Qt::Key_Down);
        controlKeys.insert("menu select", Qt::Key_Return);
        controlKeys.insert("exit from menu", Qt::Key_Backspace);
        controlKeys.insert("create soldier", Qt::Key_7);
        controlKeys.insert("create archer", Qt::Key_8);
        controlKeys.insert("create rider", Qt::Key_9);
        controlKeys.insert("create wizard", Qt::Key_0);

        gameMenu->setPos(scene()->width() - gameMenu->pixmap().width(), 0);
        town->setPos(scene()->width() - town->pixmap().width(), 0);
    }
    else
    {
        controlKeys.insert("menu", Qt::Key_Q);
        controlKeys.insert("menu up", Qt::Key_W);
        controlKeys.insert("menu down", Qt::Key_S);
        controlKeys.insert("menu select", Qt::Key_F);
        controlKeys.insert("exit from menu", Qt::Key_E);
        controlKeys.insert("create soldier", Qt::Key_1);
        controlKeys.insert("create archer", Qt::Key_2);
        controlKeys.insert("create rider", Qt::Key_3);
        controlKeys.insert("create wizard", Qt::Key_4);

        gameMenu->setPos(0, 0);
        town->setPos(0, 0);
    }

    gameMenu->setFlags(QGraphicsItem::ItemIsFocusable);
    gameMenu->setVisible(false);
    scene()->addItem(town);
    town->addHealthMoneyToScene();
    scene()->addItem(gameMenu);
    gameMenu->addMenusToScene();

    priceUpgrade.insert("Hauberk",1000);
    priceUpgrade.insert("Armor",2000);
    priceUpgrade.insert("Weapon",4000);
    priceUpgrade.insert("Quarantine",500);
    priceUpgrade.insert("Doctors",5000);
    priceUpgrade.insert("Mage_Hp",400);
    priceUpgrade.insert("Mage_Attack",400);
    priceUpgrade.insert("Arquebus",2000);
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
    pauseMenuTimer->start(5000);
}

void View::setCanMenuOpenInTrue()
{
    canMenuOpen = true;
    pauseMenuTimer->stop();
}

void View::resizeEvent(QResizeEvent *event)
{
    // В этом if настраивается sceneRect нижнего view после события View::resizeEvent

    if(bottomView)
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

        inMenuTimer->start(10000);
        menuOpen = true;
        return;
    }
    if(menuOpen)
    {
        if(event->nativeVirtualKey() == getControlKey("menu up") || event->key() == getControlKey("menu up"))
            gameMenu->setCurrentItem(true);
        if(event->nativeVirtualKey() == getControlKey("menu down") || event->key() == getControlKey("menu down"))
            gameMenu->setCurrentItem(false);
        if(event->nativeVirtualKey() == getControlKey("menu select") || event->key() == getControlKey("menu select"))
            gameMenu->processSelectAction();
        if(event->nativeVirtualKey() == getControlKey("exit from menu") || event->key() == getControlKey("exit from menu"))
            gameMenu->processExitAction();
    }

    // В следющих 4 ифах происходит создание юнита в зависимости от нажатой кнопки. В этих ифах нужно добавить и проверку на наличие нужной суммы
    // Если есть нужная сумма, то создвать юнита и обязательно кидать сигнал moneyWasted с ценой создания этого юнита
    if(event->nativeVirtualKey() == getControlKey("create soldier") || event->key() == getControlKey("create soldier"))
    {
        army->addTroop("soldier", scene());
    }
    if(event->nativeVirtualKey() == getControlKey("create archer") || event->key() == getControlKey("create archer"))
    {
        army->addTroop("archer", scene());
    }
    if(event->nativeVirtualKey() == getControlKey("create rider") || event->key() == getControlKey("create rider"))
    {
        army->addTroop("rider", scene());
    }
    if(event->nativeVirtualKey() == getControlKey("create wizard") || event->key() == getControlKey("create wizard"))
    {
        army->addTroop("mage", scene());
    }
}

bool View::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        if(((QKeyEvent *)event)->key() == Qt::Key_Escape)
            return false;

        // Проверка, что нажатая клавиша - клавиша управления игрока

        if(isControlKey(((QKeyEvent *)event)->nativeVirtualKey()) || isControlKey(((QKeyEvent *)event)->key()))
            keyPressEvent((QKeyEvent *)event);
        return true;
    }
    return QGraphicsView::event(event);
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

void View::setPriceUpgrade(QString key, int value)
{
    priceUpgrade[key] = value;
}

int View::getPriceUpgrade(QString key)
{
    return priceUpgrade[key];
}

bool View::checkControlKey(Qt::Key key)
{
    foreach (Qt::Key value, controlKeys) {
        if(value == key)
            return true;
    }
    return false;
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
