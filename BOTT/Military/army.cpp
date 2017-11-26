#include "Military/army.h"
#include "view.h"
#include "town.h"
#include <QGraphicsScene>
#include "Military/type/infantry.h"
#include "Military/type/archer.h"
#include "Military/type/raider.h"
#include "Military/type/mage.h"



Army::Army(View * parent,PoC party)
{
    this->parent = parent;
    this->party=party;

    soldier.setPixmap(QPixmap(":/images/images/Military/infantry/farmer_stand_1.png"));
    soldier.setParty(party);
    soldier.setType("soldier");
    soldier.setAtack(200);
    soldier.setHp(1000);
    soldier.setDef(0);
    soldier.setImg_Pref(":/images/images/Military/infantry/farmer_");

    archer.setPixmap(QPixmap(":/images/images/Military/archer/archer_stand_1.png"));
    archer.setParty(party);
    archer.setType("archer");
    archer.setAtack(100);
    archer.setHp(1000);
    archer.setDef(0);
    archer.setImg_Pref(":/images/images/Military/archer/archer_");

    rider.setPixmap(QPixmap(":/images/images/Military/rider/rider_stand_1.png"));
    rider.setParty(party);
    rider.setType("rider");
    rider.setAtack(200);
    rider.setHp(2000);
    rider.setDef(0);
    rider.setImg_Pref(":/images/images/Military/rider/rider_");

    mage.setPixmap(QPixmap(":/images/images/Military/mag/mag_stand_1.png"));
    mage.setParty(party);
    mage.setType("mage");
    mage.setAtack(50);
    mage.setHp(500);
    mage.setDef(0);
    mage.setImg_Pref(":/images/images/Military/mag/mag_");

}

void Army::addTroop(QString type, QGraphicsScene * scene)
{
    if(arm.size()<6)
    {
    if(type=="soldier")
    {
        if(parent->getTown()->getMoney()>=1000)
        {
        arm.resize(arm.size()+1);
        arm[arm.size()-1]=new Infantry(soldier);
        arm[arm.size()-1]->startAllTimers();
        scene->addItem(arm[arm.size()-1]);
        parent->getTown()->setMoney(parent->getTown()->getMoney()-1000);
        }
    }
    if(type=="archer")
    {
        if(parent->getTown()->getMoney()>=1000)
        {
        arm.resize(arm.size()+1);
        arm[arm.size()-1]=new Archer(archer);
        arm[arm.size()-1]->startAllTimers();
        scene->addItem(arm[arm.size()-1]);
        parent->getTown()->setMoney(parent->getTown()->getMoney()-1000);
        }
    }
    if(type=="rider")
    {
        if(parent->getTown()->getMoney()>=3000)
        {
        arm.resize(arm.size()+1);
        arm[arm.size()-1]=new Rider(rider);
        arm[arm.size()-1]->startAllTimers();
        scene->addItem(arm[arm.size()-1]);
        parent->getTown()->setMoney(parent->getTown()->getMoney()-3000);
        }
    }
    if(type=="mage")
    {
        if(parent->getTown()->getMoney()>=2000)
        {
        arm.resize(arm.size()+1);
        arm[arm.size()-1]=new Mage(mage);
        arm[arm.size()-1]->startAllTimers();
        scene->addItem(arm[arm.size()-1]);
        parent->getTown()->setMoney(parent->getTown()->getMoney()-2000);
        }
    }

    emit uniteCreated();
    }
}

Troop * Army::getTroop(int n)
{
    if(n < arm.size())
        return arm[n];
    else return NULL;
}

int Army::size()
{
    return arm.size();
}

void Army::setParty(PoC party)
{
    this->party=party;
}

void Army::deleteTroop()
{
    delete this->arm[0];

    for(int i=0;i<arm.size()-1;i++)
    {
        arm[i]=arm[i+1];
    }
    arm.resize(arm.size()-1);
}

int Army::getTownHp()
{
    return parent->getTown()->getHealth();
}

void Army::setTownHp(int hp)
{
    parent->getTown()->setHealth(hp);
}
