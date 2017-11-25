#include "Military/battlefield.h"
#include "Military/army.h"
#include <QTimer>
#include <cmath>
#include <QGraphicsScene>
#include <QDebug>

Battlefield::Battlefield()
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(Battle()));//каждую секунду проверяем есть ли войска рядом
    timer_interval = 1000;
    timer_remainingTime = 1000; //и каждую секунду бьем противника, если он близко
}

Battlefield::~Battlefield()
{
    // армии удалять не нужно, т.к. каждый view сам удалит свою армию
    delete timer;
}

void Battlefield::setScene(QGraphicsScene * scene)
{
    this->scene = scene;
}

void Battlefield::setArmies(Army * army1, Army * army2)
{
    arm1 = army1;
    arm2 = army2;
}

void Battlefield::stopAllTimers()
{
    timer_remainingTime = timer->remainingTime();
    timer->stop();
}

void Battlefield::startAllTimers()
{
    timer->start(timer_remainingTime);
}

void Battlefield::Battle()
{
    timer->start(timer_interval);

    if(arm1->size() == 0 || arm2->size() == 0)
        return;//Заглушка, пока нет атаки на город

    if(arm2->getTroop(0)->x() - arm1->getTroop(0)->x() <= 50)//если они достаточно близко, то расчитываем показатели атаки
    {
        float dmg1 = 0, dmg2 = 0;
        for(int i = 0; i < arm1->size(); i++)
        {
            if(arm1->getTroop(i)->getType() == "rider" && arm2->getTroop(0)->getType() == "archer")//Двойной урон от "противоположного" типа войск
            {
                dmg1 += arm1->getTroop(i)->getAtack() * std::pow(0.5, i) * 2;
            }
            else if(arm1->getTroop(i)->getType() == "archer" && arm2->getTroop(0)->getType() == "soldier")
            {
                dmg1 += arm1->getTroop(i)->getAtack() * std::pow(0.5, i) * 2;
            }
            else if(arm1->getTroop(i)->getType() == "soldier" && arm2->getTroop(0)->getType() == "rider")
            {
                dmg1 += arm1->getTroop(i)->getAtack() * std::pow(0.5, i) * 2;
            }
            else
                dmg1 += arm1->getTroop(i)->getAtack() * std::pow(0.5, i);
        }
        for(int i = 0; i < arm2->size(); i++)
        {
            if(arm2->getTroop(i)->getType() == "rider" && arm1->getTroop(0)->getType() == "archer")
            {
                dmg2 += arm2->getTroop(i)->getAtack() * std::pow(0.5, i) * 2;
            }
            else if(arm2->getTroop(i)->getType() == "archer" && arm1->getTroop(0)->getType() == "soldier")
            {
                dmg2 += arm2->getTroop(i)->getAtack() * std::pow(0.5, i) * 2;
            }
            else if(arm2->getTroop(i)->getType() == "soldier" && arm1->getTroop(0)->getType() == "rider")
            {
                dmg2 += arm2->getTroop(i)->getAtack() * std::pow(0.5, i) * 2;
            }
            else
                dmg2 += arm2->getTroop(i)->getAtack() * std::pow(0.5, i);
   }
   //arm1->getTroop(0)->setHp(arm1->getTroop(0)->getHp() -= dmg2);
   //arm2->getTroop(0)->setHp(arm2->getTroop(0)->getHp() -= dmg1) тут должно быть удаление солдат из армии и так далее
   //...
   }
}
