#include "town.h"
#include "view.h"
#include "message.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QDebug>

Town::Town(View * parent) : QObject()
{
    this->parent = parent;

    health = 100;
    money = 100000;
    income = 1000;
    incomeTimer = new QTimer;
    incomeTimer_interval = 5000;
    incomeTimer_remainingTime = 5000;

    healthItem = new QGraphicsTextItem();
    moneyItem = new QGraphicsTextItem();

    healthItem->setPlainText(QString::number(health) + " hp");
    healthItem->setFont(QFont("Old English Text MT", 23));
    healthItem->setDefaultTextColor(Qt::red);
    moneyItem->setPlainText(QString::number(money));
    moneyItem->setFont(QFont("Old English Text MT", 23));
    moneyItem->setDefaultTextColor(QColor(255, 246, 0));

    connect(incomeTimer, SIGNAL(timeout()), this, SLOT(addMoney()));
}

Town::~Town()
{
    delete incomeTimer;
    delete healthItem;
    delete moneyItem;
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
    background->setPixmap(QPixmap(":images/images/HP_Money.png"));
    background->setPos(x, pos().y());
    scene()->addItem(background);

    healthItem->setPos(x + 30, pos().y() - 2);
    moneyItem->setPos(x + 150, pos().y() - 2);
    scene()->addItem(healthItem);
    scene()->addItem(moneyItem);
}

void Town::setHealth(int hp)
{
    this->health = hp;
    if(health < 0)
        health = 0;
    this->healthItem->setPlainText(QString::number(health) + " hp");
}

void Town::setMoney(int mn)
{
    this->money = mn;
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
    if(money >= parent->getPriceUpgrade("Mine level up"))
    {
        emit moneyWasted(parent->getPriceUpgrade("Mine level up"));
        emit modificate();
        parent->setPriceUpgrade("Mine level up", parent->getPriceUpgrade("Mine level up") * 1.5);
        income += 3000;
    }
    else
    {
        Message message((QWidget *)(parent->parent()));
        message.setMessage("Недостаточно денег для покупки этого улучшения!");
        message.show();
        message.exec();
    }
}
