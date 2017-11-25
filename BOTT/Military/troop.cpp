#include "Military/troop.h"
#include <QTimer>

Troop::Troop()
{
    timer_interval = 1000;
    timer_remainingTime = 1000;
}

void Troop::stopAllTimers()
{
    timer_remainingTime = timer->remainingTime();
    timer->stop();
}

void Troop::startAllTimers()
{
    timer->start(timer_remainingTime);
}

float Troop::getHp()
{
    return hp;
}

float Troop::getAtack()
{
    return atack;
}

QString Troop::getType()
{
    return type;
}

void Troop::setHp(float hp)
{
    this->hp = hp;
}
