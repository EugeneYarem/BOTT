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
    isArmorImprove=false;
    isHauberkImprove=false;
    isWeaponImprove=false;
    isDoctorsImprove=false;
    isArquebusImprove=false;
    soldier=new Infantry();
    archer=new Archer();
    rider=new Rider();
    mage=new Mage();

    soldier->setPixmap(QPixmap(":/images/images/Military/infantry/farmer_stand_1.png"));
    soldier->setParty(party);
    soldier->setType("soldier");
    soldier->setAtack(200);
    soldier->setHp(1000);
    soldier->setDef(0);
    soldier->setImg_Pref(":/images/images/Military/infantry/farmer_");

    archer->setPixmap(QPixmap(":/images/images/Military/archer/archer_stand_1.png"));
    archer->setParty(party);
    archer->setType("archer");
    archer->setAtack(100);
    archer->setHp(1000);
    archer->setDef(0);
    archer->setImg_Pref(":/images/images/Military/archer/archer_");

    rider->setPixmap(QPixmap(":/images/images/Military/rider/cavalier_stand_1.png"));
    rider->setParty(party);
    rider->setType("rider");
    rider->setAtack(200);
    rider->setHp(2000);
    rider->setDef(0);
    rider->setImg_Pref(":/images/images/Military/rider/cavalier_");

    mage->setPixmap(QPixmap(":/images/images/Military/mag/mag_stand_1.png"));
    mage->setParty(party);
    mage->setType("mage");
    mage->setAtack(50);
    mage->setHp(500);
    mage->setDef(0);
    mage->setImg_Pref(":/images/images/Military/mag/mag_");

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
        arm[arm.size()-1]=new Infantry(*soldier);
        arm[arm.size()-1]->startAllTimers();
        scene->addItem(arm[arm.size()-1]);
        parent->getTown()->setMoney(parent->getTown()->getMoney()-1000);
        emit this->moneyWasted(1000);
        emit this->uniteCreated();
        }
    }
    if(type=="archer")
    {
        if(parent->getTown()->getMoney()>=1000)
        {
        arm.resize(arm.size()+1);
        arm[arm.size()-1]=new Archer(*archer);
        arm[arm.size()-1]->startAllTimers();
        scene->addItem(arm[arm.size()-1]);
        parent->getTown()->setMoney(parent->getTown()->getMoney()-1000);
        emit this->moneyWasted(1000);
        emit this->uniteCreated();
        }
    }
    if(type=="rider")
    {
        if(parent->getTown()->getMoney()>=3000)
        {
        arm.resize(arm.size()+1);
        arm[arm.size()-1]=new Rider(*rider);
        arm[arm.size()-1]->startAllTimers();
        scene->addItem(arm[arm.size()-1]);
        parent->getTown()->setMoney(parent->getTown()->getMoney()-3000);
        emit this->moneyWasted(3000);
        emit this->uniteCreated();
        }
    }
    if(type=="mage")
    {
        if(parent->getTown()->getMoney()>=2000)
        {
        arm.resize(arm.size()+1);
        arm[arm.size()-1]=new Mage(*mage);
        arm[arm.size()-1]->startAllTimers();
        scene->addItem(arm[arm.size()-1]);
        parent->getTown()->setMoney(parent->getTown()->getMoney()-2000);
        emit this->moneyWasted(2000);
        emit this->uniteCreated();
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

void Army::improveHauberk()
{
    if(!isHauberkImprove)
    {
    if(parent->getTown()->getMoney()>=parent->getPriceUpgrade("Hauberk"))
    {
    QString str=soldier->getImg_pref();
    str.resize(str.size()-7);
    str+="soldier_";
    soldier->setImg_Pref(str);
    soldier->setAtack(soldier->getAtack()+100);
    soldier->setHp(soldier->getHp()+300);
    soldier->setPixmap(QPixmap(soldier->getImg_pref()+"stand_1"));
    for(int i=0;i<arm.size();i++)
    {
        if(arm[i]->getType()=="soldier")
        {
            arm[i]->setImg_Pref(str);
            arm[i]->setAtack(arm[i]->getAtack()+100);
            arm[i]->setHp(arm[i]->getHp()+300);
        }
    }
    parent->getTown()->setMoney(parent->getTown()->getMoney()-parent->getPriceUpgrade("Hauberk"));
    moneyWasted(parent->getPriceUpgrade("Hauberk"));
    isHauberkImprove=true;
    }
    }
}

void Army::improveArmor()
{
    if(isHauberkImprove&&!isArmorImprove)
    {
        if(parent->getTown()->getMoney()>=parent->getPriceUpgrade("Armor"))
        {
    QString str=soldier->getImg_pref();
    str.resize(str.size()-8);
    str+="knight_";
    soldier->setImg_Pref(str);

    soldier->setAtack(soldier->getAtack()+100);
    soldier->setHp(soldier->getHp()+300);
    soldier->setPixmap(QPixmap(soldier->getImg_pref()+"stand_1"));

    for(int i=0;i<arm.size();i++)
    {
        if(arm[i]->getType()=="soldier")
        {
            arm[i]->setImg_Pref(str);
            arm[i]->setAtack(arm[i]->getAtack()+100);
            arm[i]->setHp(arm[i]->getHp()+300);
        }
    }
    isArmorImprove=true;
    parent->getTown()->setMoney(parent->getTown()->getMoney()-parent->getPriceUpgrade("Armor"));
    moneyWasted(parent->getPriceUpgrade("Armor"));
    }
    }


}

void Army::improveWeapon()
{
    if(isArmorImprove&&!isWeaponImprove)
    {
        if(parent->getTown()->getMoney()>=parent->getPriceUpgrade("Weapon"))
        {
    QString str=rider->getImg_pref();
    str.resize(str.size()-9);
    str+="rider_";
    rider->setImg_Pref(str);

    rider->setAtack(rider->getAtack()+200);
    rider->setHp(rider->getHp()+600);
    rider->setPixmap(QPixmap(rider->getImg_pref()+"stand_1"));

    for(int i=0;i<arm.size();i++)
    {
        if(arm[i]->getType()=="rider")
        {
            arm[i]->setImg_Pref(str);
        }
    }

        parent->getTown()->setMoney(parent->getTown()->getMoney()-parent->getPriceUpgrade("Weapon"));
        moneyWasted(parent->getPriceUpgrade("Weapon"));
        }
    }

}

void Army::improveQuarantine()
{
    if(parent->getTown()->getMoney()>=parent->getPriceUpgrade("Quarantine"))
    {
    soldier->setHp(soldier->getHp()+200);
    rider->setHp(soldier->getHp()+200);
    archer->setHp(soldier->getHp()+200);
    mage->setHp(soldier->getHp()+200);

    for(int i=0;i<arm.size();i++)
    {
        arm[i]->setHp(arm[i]->getHp()+200);
    }

    int price=parent->getPriceUpgrade("Quarantine");
    parent->getTown()->setMoney(parent->getTown()->getMoney()-price);
    moneyWasted(parent->getPriceUpgrade("Quarantine"));
    parent->setPriceUpgrade("Quarantine",price*1.5);
    }
}

void Army::improveDoctors()
{
    if(!isDoctorsImprove)
    {
    if(parent->getTown()->getMoney()>=parent->getPriceUpgrade("Doctors"))
    {
    soldier->setHp(soldier->getHp()*2);
    rider->setHp(soldier->getHp()*2);
    archer->setHp(soldier->getHp()*2);
    mage->setHp(soldier->getHp()*2);

    for(int i=0;i<arm.size();i++)
    {
        arm[i]->setHp(arm[i]->getHp()*2);
    }
    int price=parent->getPriceUpgrade("Doctors");
    parent->getTown()->setMoney(parent->getTown()->getMoney()-price);
    moneyWasted(parent->getPriceUpgrade("Doctors"));
    }
    }
}

void Army::improveArquebus()
{
    if(!isArquebusImprove)
    {
    if(parent->getTown()->getMoney()>=parent->getPriceUpgrade("Arquebus"))
    {

    QString str=archer->getImg_pref();
    str.resize(str.size()-7);
    str+="crossbow_";
    archer->setImg_Pref(str);

    archer->setAtack(archer->getAtack()+200);
    archer->setHp(archer->getHp()+400);
    archer->setPixmap(QPixmap(rider->getImg_pref()+"stand_1"));

    for(int i=0;i<arm.size();i++)
    {
        if(arm[i]->getType()=="archer")
        {
            arm[i]->setImg_Pref(str);
        }
    }
    parent->getTown()->setMoney(parent->getTown()->getMoney()-parent->getPriceUpgrade("Arquebus"));
    moneyWasted(parent->getPriceUpgrade("Arquebus"));
    }
    }

}

void Army::increaseMageHitPoint()
{
    if(parent->getTown()->getMoney()>=parent->getPriceUpgrade("Quarantine"))
    {
    mage->setHp(mage->getHp()+300);

    for(int i=0;i<arm.size();i++)
    {
        if(arm[i]->getType()=="mage")
        arm[i]->setHp(arm[i]->getHp()+300);
    }
    int price=parent->getPriceUpgrade("Quarantine");
    parent->getTown()->setMoney(parent->getTown()->getMoney()-price);
    moneyWasted(parent->getPriceUpgrade("Quarantine"));
    parent->setPriceUpgrade("Quarantine",price*1.5);
    }
}

void Army::increaseMageAttack()
{
    if(parent->getTown()->getMoney()>=parent->getPriceUpgrade("Quarantine"))
    {
    mage->setHp(mage->getAtack()+30);

    for(int i=0;i<arm.size();i++)
    {
        if(arm[i]->getType()=="mage")
        arm[i]->setHp(arm[i]->getAtack()+30);
    }
    int price=parent->getPriceUpgrade("Quarantine");
    parent->getTown()->setMoney(parent->getTown()->getMoney()-price);
    moneyWasted(parent->getPriceUpgrade("Quarantine"));
    parent->setPriceUpgrade("Quarantine",price*1.5);
    }
}


