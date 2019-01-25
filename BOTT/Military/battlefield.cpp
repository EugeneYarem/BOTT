#include "constants.h"
#include "Military/army.h"
#include "Military/battlefield.h"
#include <QTimer>




Battlefield::Battlefield()
{
    timer = new QTimer();
    timer_B = new QTimer();

    connect(timer, &QTimer::timeout, this, &Battlefield::armyControl); // каждую секунду проверяем есть ли войска рядом
    connect(timer_B, &QTimer::timeout, this, &Battlefield::battle);
    timer_interval = ARMY_CONTROL_TIMER_INTERVAL;
    timer_remainingTime = ARMY_CONTROL_TIMER_INTERVAL;
    timerB_interval = BATTLE_TIMER_INTERVAL;
    timerB_remainingTime = BATTLE_TIMER_INTERVAL;
}

Battlefield::~Battlefield()
{
    // армии удалять не нужно, т.к. каждый view сам удалит свою армию
    delete timer;
    delete timer_B;
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
    timerB_remainingTime = timer_B->remainingTime();
    timer_B->stop();
    arm1->stopAllTimers();
    arm2->stopAllTimers();
}

double Battlefield::countArmyAttack(Army * arm1, Army * arm2) const
{
    double dmg = 0;

    for(int i = 0; i < arm1->size(); i++)
    {
        if(arm1->getTroop(i)->getType() == "rider" && arm2->getTroop(0)->getType() == "archer")//Двойной урон от "противоположного" типа войск
            dmg += arm1->getTroop(i)->getAttack() * 2 * std::pow(0.5, i);
        else if(arm1->getTroop(i)->getType() == "archer" && arm2->getTroop(0)->getType() == "soldier")
            dmg += arm1->getTroop(i)->getAttack() * 2 * std::pow(0.5, i);
        else if(arm1->getTroop(i)->getType() == "soldier" && arm2->getTroop(0)->getType() == "rider")
            dmg += arm1->getTroop(i)->getAttack() * 2 * std::pow(0.5, i);
        else
            dmg += arm1->getTroop(i)->getAttack() * std::pow(0.5, i);
    }

    return dmg;
}

void Battlefield::startAllTimers() const
{
    timer->start(timer_remainingTime);
    timer_B->start(timerB_remainingTime);
    arm1->startAllTimers();
    arm2->startAllTimers();
}

void Battlefield::clearStart()
{
    arm1->clearStart();
    arm2->clearStart();
}

void Battlefield::battle() const
{  
    this->timer_B->start(timerB_interval);

    if(arm1->size() != 0 && arm2->size() != 0)
    {
        if(arm2->getTroop(0)->x() - arm1->getTroop(0)->x() <= DISTANCE_BETWEEN_ARMIES_FOR_START_BATTLE)//если они достаточно близко, то расчитываем показатели атаки
        {
            double dmg1 = countArmyAttack(this->arm1, this->arm2),
                   dmg2 = countArmyAttack(this->arm2, this->arm1);

            double buf = dmg2;
            dmg2 -= arm1->getTroop(0)->getHp();
            arm1->getTroop(0)->setHp(arm1->getTroop(0)->getHp() - buf);

            if(arm1->getTroop(0)->getHp() <= 0)
            {
                this->arm1->deleteTroop();
                if(arm1->size() >= 1)
                      arm1->getTroop(0)->setHp(arm1->getTroop(0)->getHp() - dmg2);
            }
            buf = dmg1;
            dmg1 -= arm2->getTroop(0)->getHp();
            arm2->getTroop(0)->setHp(arm2->getTroop(0)->getHp() - buf);

            if(arm2->getTroop(0)->getHp() <= 0)
            {
                this->arm2->deleteTroop();
                if(arm2->size() >= 1)
                    arm2->getTroop(0)->setHp(arm2->getTroop(0)->getHp() - dmg1);
            }
        }
    }
    else
    {
        if(arm1->size() != 0)
        {
            if(arm1->getTroop(0)->x() >= DISTANCE_TO_TOWN2_TO_ATTACK)
            {
                double dmg = 0;
                for(int i = 0; i < arm1->size(); i++)
                    dmg += arm1->getTroop(i)->getAttack() * ATTACK_TOWN_COEFFICIENT;

                arm2->setTownHp(static_cast<int>(arm2->getTownHp() - dmg));
            }
        }
        else if(arm2->size() != 0)
        {
            if(arm2->getTroop(0)->x() <= DISTANCE_TO_TOWN1_TO_ATTACK)
            {
                double dmg = 0;
                for(int i = 0; i < arm2->size(); i++)
                    dmg += arm2->getTroop(i)->getAttack() * ATTACK_TOWN_COEFFICIENT;

                arm1->setTownHp(static_cast<int>(arm1->getTownHp() - dmg));
            }
        }
    }

}

void Battlefield::armyControl() const
{
    timer->start(timer_interval);
    if(arm1->size() != 0)
    {
        if(arm1->getTroop(0)->x() >= DISTANCE_TO_TOWN2_TO_ATTACK)
            arm1->getTroop(0)->setStatus(Status::Attack);
        else if(arm2->size() != 0)
        {
           if(arm1->getTroop(0)->x() >= arm2->getTroop(0)->x() - DISTANCE_BETWEEN_TROOPS_FOR_START_BATTLE)
               arm1->getTroop(0)->setStatus(Status::Attack);
           else arm1->getTroop(0)->setStatus(Status::Run);
        }
        else
        {
            arm1->getTroop(0)->setStatus(Status::Run);
            timer_B->start(timerB_interval);
        }

        for(int i = 1; i < arm1->size(); i++)
        {
           if(arm1->getTroop(i)->x() >= (arm1->getTroop(i - 1)->x() - 80))
               arm1->getTroop(i)->setStatus(Status::Stand);
           else arm1->getTroop(i)->setStatus(Status::Run);
        }
    }

    if(arm2->size() != 0)
    {
        if(arm2->getTroop(0)->x() <= 290)
            arm2->getTroop(0)->setStatus(Status::Attack);
        else if(arm1->size() != 0)
        {
           if(arm2->getTroop(0)->x() <= arm1->getTroop(0)->x() + DISTANCE_BETWEEN_TROOPS_FOR_START_BATTLE)
               arm2->getTroop(0)->setStatus(Status::Attack);
           else arm2->getTroop(0)->setStatus(Status::Run);
        }
        else arm2->getTroop(0)->setStatus(Status::Run);

        for(int i = 1; i < arm2->size(); i++)
        {
            int dist;
            if(arm2->getTroop(i - 1)->getType() == "rider" && arm2->getTroop(i)->getType() != "rider")
                dist = DISTANCE_BETWEEN_TROOPS_FOR_START_BATTLE + 30;
            else dist = DISTANCE_BETWEEN_TROOPS_FOR_START_BATTLE;

            if(arm2->getTroop(i)->x() <= (arm2->getTroop(i - 1)->x() + dist))
                arm2->getTroop(i)->setStatus(Status::Stand);
            else arm2->getTroop(i)->setStatus(Status::Run);
        }
    }
}
