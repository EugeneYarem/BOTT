#include "town.h"
#include <QTimer>
#include <QDebug>

Town::Town() : QObject()
{
    health = 100;

    setPixmap(QPixmap(":images/images/towns/town.png"));

    incomeTimer = new QTimer;
    incomeTimer->start(5000);
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

void Town::damage(int damage)
{
    health -= damage;
}

void Town::addMoney()
{
    money += income;
}

void Town::setNewIncome()
{
    income += 100;
}
