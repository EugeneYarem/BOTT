#include "town.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QDebug>

Town::Town() : QObject()
{
    health = 100;
    money = 1000;
    income = 1000;
    incomeTimer = new QTimer;
    incomeTimer_interval = 5000;
    incomeTimer_remainingTime = 5000;

    healthItem = new QGraphicsTextItem();
    moneyItem = new QGraphicsTextItem();

    healthItem->setPlainText(QString::number(health) + " hp");
    healthItem->setFont(QFont("Old English Text MT", 23));
    healthItem->setDefaultTextColor(Qt::darkRed);
    moneyItem->setPlainText(QString::number(money));
    moneyItem->setFont(QFont("Old English Text MT", 23));
    moneyItem->setDefaultTextColor(Qt::darkYellow);

    connect(incomeTimer, SIGNAL(timeout()), this, SLOT(addMoney()));
}

int Town::getHealth()
{
    return health;
}

int Town::getMoney()
{
    return money;
}

void Town::stopAllTimers()
{
    incomeTimer_remainingTime = incomeTimer->remainingTime();
    incomeTimer->stop();
}

void Town::startAllTimers()
{
    incomeTimer->start(incomeTimer_remainingTime);
}

void Town::addHealthMoneyToScene()
{
    int x = pos().x();
    if(x != 0)
        x = scene()->width() - 300;

    QGraphicsPixmapItem * background = new QGraphicsPixmapItem();
    background->setPixmap(QPixmap(":images/images/hpMoneyBG.png"));
    background->setPos(x, pos().y());
    scene()->addItem(background);

    healthItem->setPos(x, pos().y());
    moneyItem->setPos(x + 150, pos().y());
    scene()->addItem(healthItem);
    scene()->addItem(moneyItem);
}

void Town::setHealth(int hp)
{
    this->health=hp;
    if(health<0)health=0;
    this->healthItem->setPlainText(QString::number(health) + " hp");
}

void Town::setMoney(int mn)
{
    this->money=mn;
    this->moneyItem->setPlainText(QString::number(money));
}

void Town::damage(int damage)
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
    // Тут должна быть проверка на то, хватает ли денег на улучшение шахты (увеличение дохода)
    income += 100;

    //emit moneyWasted(int); Тут должен выкидываться этот сигнал с ценой улучшения
}
