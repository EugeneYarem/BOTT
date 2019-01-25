#include "constants.h"
#include "town.h"
#include "view.h"
#include <QTimer>


Town::Town(View * parent) : QObject()
{
    this->parent = parent;

    health = TOWN_HEALTH;
    money = TOWN_MONEY;
    income = TOWN_INCOME;
    incomeTimer = new QTimer;
    incomeTimer_interval = INCOME_INTERVAL;
    incomeTimer_remainingTime = INCOME_INTERVAL;

    healthItem = new QGraphicsTextItem();
    moneyItem = new QGraphicsTextItem();

    healthItem->setPlainText(QString::number(health) + " hp");
    healthItem->setFont(QFont("Old English Text MT", 23));
    healthItem->setDefaultTextColor(Qt::red);
    moneyItem->setPlainText(QString::number(money));
    moneyItem->setFont(QFont("Old English Text MT", 23));
    moneyItem->setDefaultTextColor(QColor(255, 246, 0));

    connect(incomeTimer, &QTimer::timeout, this, &Town::addMoney);
}

Town::~Town()
{
    delete incomeTimer;
    delete healthItem;
    delete moneyItem;
}

int Town::getHealth() const
{
    return health;
}

int Town::getMoney() const
{
    return money;
}

void Town::stopAllTimers()
{
    incomeTimer_remainingTime = incomeTimer->remainingTime();
    incomeTimer->stop();
}

void Town::startAllTimers() const
{
    incomeTimer->start(incomeTimer_remainingTime);
}

void Town::addHealthMoneyToScene()
{
    long int x = static_cast<long int>(pos().x());
    if(x != 0)
        x = static_cast<long int>(scene()->width() - 300);

    QGraphicsPixmapItem * background = new QGraphicsPixmapItem();
    background->setPixmap(QPixmap(":images/images/HP_Money.png"));
    background->setPos(x, pos().y());
    scene()->addItem(background);

    healthItem->setPos(x + 30, pos().y() - 2);
    moneyItem->setPos(x + 150, pos().y() - 2);

    scene()->addItem(healthItem);
    scene()->addItem(moneyItem);
}

void Town::setHealth(const int & hp)
{
    this->health = hp;
    if(health <= 0)
    {
        health = 0;
        emit loose();
    }
    this->healthItem->setPlainText(QString::number(health) + " hp");
}

void Town::setMoney(const int & mn)
{
    this->money = mn;
    this->moneyItem->setPlainText(QString::number(money));
}

void Town::clearStart()
{
    health = TOWN_HEALTH;
    money = TOWN_MONEY;
    income = TOWN_INCOME;
    incomeTimer_interval = INCOME_INTERVAL;
    incomeTimer_remainingTime = INCOME_INTERVAL;
    this->healthItem->setPlainText(QString::number(health) + " hp");
    this->moneyItem->setPlainText(QString::number(money));
    startAllTimers();
}

void Town::damage(const int & damage)
{
    health -= damage;
    healthItem->setPlainText(QString::number(health) + " hp");
}

void Town::addMoney()
{
    money += income;
    moneyItem->setPlainText(QString::number(money));
    incomeTimer->start(incomeTimer_interval);
    emit moneyEarned(income);
}

void Town::setNewIncome()
{
    if(money >= parent->getPriceUpgrade("Mine level up"))
    {
        emit moneyWasted(parent->getPriceUpgrade("Mine level up"));
        emit modificate();
        parent->setPriceUpgrade("Mine level up", static_cast<int>(parent->getPriceUpgrade("Mine level up") * INCOME_UPGRADE_NEW_PRICE_COEFFICIENT));
        income += INCOME_UPGRADE;
    }
    else
    {
        emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
    }
}
