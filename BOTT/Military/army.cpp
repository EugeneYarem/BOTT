#include "view.h"
#include "town.h"
#include "Military/army.h"
#include "Military/type/infantry.h"
#include "Military/type/archer.h"
#include "Military/type/raider.h"
#include "Military/type/mage.h"
#include "message.h"
#include <QGraphicsScene>

Army::Army(View * parent, PoC party)
{
    this->parent = parent;
    this->party = party;

    soldier = new Infantry();
    archer = new Archer();
    rider = new Rider();
    mage = new Mage();

    ClearStart();
}

Army::~Army()
{
    delete soldier;
    delete archer;
    delete rider;
    delete mage;

    for(int i = 0; i < arm.size(); i++)
        delete arm[i];

    arm.clear();
}

void Army::addTroop(QString type, QGraphicsScene * scene)
{
    if(arm.size() < 6)
    {
        if(type == "soldier")
        {
            if(parent->getTown()->getMoney() >= 1000)
            {
                arm.resize(arm.size() + 1);
                arm[arm.size() - 1] = new Infantry(* soldier);
                arm[arm.size() - 1]->startAllTimers();
                scene->addItem(arm[arm.size() - 1]);
                scene->addItem(arm[arm.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - 1000);
                emit this->moneyWasted(1000);
                emit this->uniteCreated();
            }
        }
        if(type == "archer")
        {
            if(parent->getTown()->getMoney() >= 1000)
            {
                arm.resize(arm.size() + 1);
                arm[arm.size() - 1] = new Archer(* archer);
                arm[arm.size() - 1]->startAllTimers();
                scene->addItem(arm[arm.size() - 1]);
                scene->addItem(arm[arm.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - 1000);
                emit this->moneyWasted(1000);
                emit this->uniteCreated();
            }
        }
        if(type == "rider")
        {
            if(parent->getTown()->getMoney() >= 3000)
            {
                arm.resize(arm.size() + 1);
                arm[arm.size() - 1] = new Rider(* rider);
                arm[arm.size() - 1]->startAllTimers();
                scene->addItem(arm[arm.size() - 1]);
                scene->addItem(arm[arm.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - 3000);
                emit this->moneyWasted(3000);
                emit this->uniteCreated();
            }
        }
        if(type == "mage")
        {
            if(parent->getTown()->getMoney() >= 2000)
            {
                arm.resize(arm.size() + 1);
                arm[arm.size() - 1] = new Mage(* mage);
                arm[arm.size() - 1]->startAllTimers();
                scene->addItem(arm[arm.size() - 1]);
                scene->addItem(arm[arm.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - 2000);
                emit this->moneyWasted(2000);
                emit this->uniteCreated();
            }
        }
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
    this->party = party;
}

void Army::deleteTroop()
{
    delete this->arm[0];

    for(int i = 0; i < arm.size() - 1; i++)
    {
        arm[i] = arm[i + 1];
    }
    arm.resize(arm.size() - 1);
}

int Army::getTownHp()
{
    return parent->getTown()->getHealth();
}

void Army::setTownHp(int hp)
{
    parent->getTown()->setHealth(hp);
}

void Army::ClearStart()
{
    isArmorImprove = false;
    isHauberkImprove = false;
    isWeaponImprove = false;
    isDoctorsImprove = false;
    isArquebusImprove = false;

    soldier->setPixmap(QPixmap(":/images/images/Military/infantry/farmer_stand_1.png"));
    soldier->setParty(party);
    soldier->setType("soldier");
    soldier->setAtack(100);
    soldier->InitialText();
    soldier->setHp(1000);
    soldier->setDef(0);
    soldier->setImg_Pref(":/images/images/Military/infantry/farmer_");
    soldier->setTime_interval(100);
    soldier->setTime_remainingTime(100);

    archer->setPixmap(QPixmap(":/images/images/Military/archer/archer_stand_1.png"));
    archer->setParty(party);
    archer->setType("archer");
    archer->setAtack(70);
    archer->InitialText();
    archer->setHp(1000);
    archer->setDef(0);
    archer->setImg_Pref(":/images/images/Military/archer/archer_");
    archer->setTime_interval(100);
    archer->setTime_remainingTime(100);

    rider->setPixmap(QPixmap(":/images/images/Military/rider/cavalier_stand_1.png"));
    rider->setParty(party);
    rider->setType("rider");
    rider->setAtack(140);
    rider->InitialText();
    rider->setHp(1500);
    rider->setDef(0);
    rider->setImg_Pref(":/images/images/Military/rider/cavalier_");
    rider->setTime_interval(100);
    rider->setTime_remainingTime(100);

    mage->setPixmap(QPixmap(":/images/images/Military/mag/mag_stand_1.png"));
    mage->setParty(party);
    mage->setType("mage");
    mage->setAtack(50);
    mage->InitialText();
    mage->setHp(500);
    mage->setDef(0);
    mage->setImg_Pref(":/images/images/Military/mag/mag_");
    mage->setTime_interval(100);
    mage->setTime_remainingTime(100);

    for(int i = 0; i < arm.size(); i++)
        delete arm[i];

    arm.clear();
}

void Army::startAllTimers()
{
    for(int i = 0; i < arm.size(); i++)
        arm[i]->startAllTimers();
}

void Army::stopAllTimers()
{
    for(int i = 0; i < arm.size(); i++)
        arm[i]->stopAllTimers();
}

void Army::improveHauberk()
{
    if(!isHauberkImprove)
    {
        if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Hauberk"))
        {
            QString str = soldier->getImg_pref();
            str.resize(str.size() - 7);
            str += "soldier_";
            soldier->setImg_Pref(str);
            soldier->setAtack(soldier->getAtack() + 30);
            soldier->setHp(soldier->getHp() + 250);
            soldier->setPixmap(QPixmap(soldier->getImg_pref() + "stand_1.png"));

            for(int i = 0; i < arm.size(); i++)
            {
                if(arm[i]->getType() == "soldier")
                {
                    arm[i]->setImg_Pref(str);
                    arm[i]->setAtack(arm[i]->getAtack() + 30);
                    arm[i]->setHp(arm[i]->getHp() + 250);
                }
            }
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Hauberk"));
            emit moneyWasted(parent->getPriceUpgrade("Hauberk"));
            emit modificate();
            isHauberkImprove = true;
            parent->deleteCurrentMenuItem();
        }
        else
        {
            Message message((QWidget *)(parent->parent()));
            message.setMessage("Недостаточно денег для покупки этого улучшения!");
            message.show();
            message.exec();
        }
    }
}

void Army::improveArmor()
{
    if(isHauberkImprove && !isArmorImprove)
    {
        if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Armor"))
        {
            QString str = soldier->getImg_pref();
            str.resize(str.size() - 8);
            str += "knight_";
            soldier->setImg_Pref(str);
            soldier->setAtack(soldier->getAtack() + 20);
            soldier->setHp(soldier->getHp() + 300);
            soldier->setPixmap(QPixmap(soldier->getImg_pref() + "stand_1.png"));

            for(int i = 0; i < arm.size(); i++)
            {
                if(arm[i]->getType() == "soldier")
                {
                    arm[i]->setImg_Pref(str);
                    arm[i]->setAtack(arm[i]->getAtack() + 20);
                    arm[i]->setHp(arm[i]->getHp() + 300);
                }
            }
            isArmorImprove = true;
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Armor"));
            emit moneyWasted(parent->getPriceUpgrade("Armor"));
            emit modificate();
            parent->deleteCurrentMenuItem();
        }
        else
        {
            Message message((QWidget *)(parent->parent()));
            message.setMessage("Недостаточно денег для покупки этого улучшения!");
            message.show();
            message.exec();
        }
    }
}

void Army::improveWeapon()
{
    if(isArmorImprove && !isWeaponImprove)
    {
        if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Weapon"))
        {
            QString str = rider->getImg_pref();
            str.resize(str.size() - 9);
            str += "rider_";
            rider->setImg_Pref(str);
            rider->setAtack(rider->getAtack() + 60);
            rider->setHp(rider->getHp() + 600);
            rider->setPixmap(QPixmap(rider->getImg_pref() + "stand_1.png"));
            soldier->setAtack(soldier->getAtack() + 40);

            for(int i = 0; i < arm.size(); i++)
            {
                if(arm[i]->getType() == "rider")
                {
                    arm[i]->setImg_Pref(str);
                    arm[i]->setAtack(arm[i]->getAtack() + 60);
                    arm[i]->setHp(arm[i]->getHp() + 600);
                }
                if(arm[i]->getType() == "soldier")
                {
                    arm[i]->setAtack(arm[i]->getAtack() + 40);
                }
            }
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Weapon"));
            emit moneyWasted(parent->getPriceUpgrade("Weapon"));
            emit modificate();
            isWeaponImprove = true;
            parent->deleteCurrentMenuItem();
        }
        else
        {
            Message message((QWidget *)(parent->parent()));
            message.setMessage("Недостаточно денег для покупки этого улучшения!");
            message.show();
            message.exec();
        }
    }
}

void Army::improveQuarantine()
{
    if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Quarantine"))
    {
        soldier->setHp(soldier->getHp() + 50);
        rider->setHp(soldier->getHp() + 50);
        archer->setHp(soldier->getHp() + 50);
        mage->setHp(soldier->getHp() + 50);

        for(int i = 0; i < arm.size(); i++)
        {
            arm[i]->setHp(arm[i]->getHp() + 50);
        }
        int price = parent->getPriceUpgrade("Quarantine");
        parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
        emit moneyWasted(parent->getPriceUpgrade("Quarantine"));
        emit modificate();
        parent->setPriceUpgrade("Quarantine", price * 1.6);
    }
    else
    {
        Message message((QWidget *)(parent->parent()));
        message.setMessage("Недостаточно денег для покупки этого улучшения!");
        message.show();
        message.exec();
    }
}

void Army::improveDoctors()
{
    if(!isDoctorsImprove)
    {
        if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Doctors"))
        {
            soldier->setHp(soldier->getHp() * 2);
            rider->setHp(soldier->getHp() * 2);
            archer->setHp(soldier->getHp() * 2);
            mage->setHp(soldier->getHp() * 2);

            for(int i = 0; i < arm.size(); i++)
            {
                arm[i]->setHp(arm[i]->getHp() * 2);
            }
            int price = parent->getPriceUpgrade("Doctors");
            parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
            emit moneyWasted(parent->getPriceUpgrade("Doctors"));
            emit modificate();
            isDoctorsImprove = true;
            parent->deleteCurrentMenuItem();
        }
        else
        {
            Message message((QWidget *)(parent->parent()));
            message.setMessage("Недостаточно денег для покупки этого улучшения!");
            message.show();
            message.exec();
        }
    }
}

void Army::improveArquebus()
{
    if(!isArquebusImprove)
    {
        if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Arquebus"))
        {
            QString str = archer->getImg_pref();
            str.resize(str.size() - 7);
            str += "crossbow_";
            archer->setImg_Pref(str);

            archer->setAtack(archer->getAtack() + 100);
            archer->setHp(archer->getHp() + 400);
            archer->setPixmap(QPixmap(archer->getImg_pref() + "stand_1.png"));

            for(int i = 0; i < arm.size(); i++)
            {
                if(arm[i]->getType() == "archer")
                {
                    arm[i]->setImg_Pref(str);
                    arm[i]->setAtack(arm[i]->getAtack() + 100);
                    arm[i]->setHp(arm[i]->getHp() + 400);
                }
            }
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Arquebus"));
            emit moneyWasted(parent->getPriceUpgrade("Arquebus"));
            emit modificate();
            isArquebusImprove = true;
            parent->deleteCurrentMenuItem();
        }
        else
        {
            Message message((QWidget *)(parent->parent()));
            message.setMessage("Недостаточно денег для покупки этого улучшения!");
            message.show();
            message.exec();
        }
    }
}

void Army::increaseMageHitPoint()
{
    if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Mage_Hp"))
    {
        mage->setHp(mage->getHp() + 300);

        for(int i = 0; i < arm.size(); i++)
        {
            if(arm[i]->getType() == "mage")
                arm[i]->setHp(arm[i]->getHp() + 300);
        }
        int price = parent->getPriceUpgrade("Mage_Hp");
        parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
        emit moneyWasted(parent->getPriceUpgrade("Mage_Hp"));
        emit modificate();
        parent->setPriceUpgrade("Mage_Hp", price * 1.3);
    }
    else
    {
        Message message((QWidget *)(parent->parent()));
        message.setMessage("Недостаточно денег для покупки этого улучшения!");
        message.show();
        message.exec();
    }
}

void Army::increaseMageAttack()
{
    if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Mage_Attack"))
    {
        mage->setAtack(mage->getAtack() + 30);

        for(int i = 0; i < arm.size(); i++)
        {
            if(arm[i]->getType() == "mage")
                arm[i]->setAtack(arm[i]->getAtack() + 30);
        }
        int price = parent->getPriceUpgrade("Mage_Attack");
        parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
        emit moneyWasted(parent->getPriceUpgrade("Mage_Attack"));
        emit modificate();
        parent->setPriceUpgrade("Mage_Attack", price * 1.3);
    }
    else
    {
        Message message((QWidget *)(parent->parent()));
        message.setMessage("Недостаточно денег для покупки этого улучшения!");
        message.show();
        message.exec();
    }
}
