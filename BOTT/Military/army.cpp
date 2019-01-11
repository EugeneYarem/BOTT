#include "Military/army.h"
#include "Military/type/archer.h"
#include "Military/type/infantry.h"
#include "Military/type/mage.h"
#include "Military/type/raider.h"
#include "town.h"
#include "view.h"


Army::Army(View * parent, ConflictSide side)
{
    this->parent = parent;
    this->side = side;

    soldier = new Infantry();
    archer = new Archer();
    rider = new Rider();
    mage = new Mage();

    clearStart();
}

Army::~Army()
{
    delete soldier;
    delete archer;
    delete rider;
    delete mage;

    for(int i = 0; i < army.size(); i++)
        delete army[i];

    army.clear();
}

void Army::addTroop(QString type, QGraphicsScene * scene)
{
    if(army.size() < 6)
    {
        if(type == "soldier")
        {
            if(parent->getTown()->getMoney() >= 1000)
            {
                army.resize(army.size() + 1);
                army[army.size() - 1] = new Infantry(* soldier);
                army[army.size() - 1]->startAllTimers();
                scene->addItem(army[army.size() - 1]);
                scene->addItem(army[army.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - 1000);
                emit this->moneyWasted(1000);
                emit this->uniteCreated();
            }
        }
        if(type == "archer")
        {
            if(parent->getTown()->getMoney() >= 1000)
            {
                army.resize(army.size() + 1);
                army[army.size() - 1] = new Archer(* archer);
                army[army.size() - 1]->startAllTimers();
                scene->addItem(army[army.size() - 1]);
                scene->addItem(army[army.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - 1000);
                emit this->moneyWasted(1000);
                emit this->uniteCreated();
            }
        }
        if(type == "rider")
        {
            if(parent->getTown()->getMoney() >= 3000)
            {
                army.resize(army.size() + 1);
                army[army.size() - 1] = new Rider(* rider);
                army[army.size() - 1]->startAllTimers();
                scene->addItem(army[army.size() - 1]);
                scene->addItem(army[army.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - 3000);
                emit this->moneyWasted(3000);
                emit this->uniteCreated();
            }
        }
        if(type == "mage")
        {
            if(parent->getTown()->getMoney() >= 2000)
            {
                army.resize(army.size() + 1);
                army[army.size() - 1] = new Mage(* mage);
                army[army.size() - 1]->startAllTimers();
                scene->addItem(army[army.size() - 1]);
                scene->addItem(army[army.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - 2000);
                emit this->moneyWasted(2000);
                emit this->uniteCreated();
            }
        }
    }
}

Troop * Army::getTroop(int n)
{
    if(n < army.size())
        return army[n];
    else return nullptr;
}

int Army::size()
{
    return army.size();
}

void Army::deleteTroop()
{
    delete this->army[0];

    for(int i = 0; i < army.size() - 1; i++)
    {
        army[i] = army[i + 1];
    }
    army.resize(army.size() - 1);
}

int Army::getTownHp()
{
    return parent->getTown()->getHealth();
}

void Army::setTownHp(int hp)
{
    parent->getTown()->setHealth(hp);
}

void Army::configureTroop(Troop * troop, QString pixmapFileName, QString type, int attack, int hp, QString imgPref, int interval, int remainingTime)
{
    troop->setPixmap(QPixmap( pixmapFileName ));
    troop->setSide( this->side );
    troop->setType( type );
    troop->setAttack(attack);
    troop->initialText();
    troop->setHp(hp);
    troop->setImgPrefix(imgPref);
    troop->setTimerInterval(interval);
    troop->setTimerRemainingTime(remainingTime);
}

void Army::clearStart()
{
    isArmorImprove = false;
    isHauberkImprove = false;
    isWeaponImprove = false;
    isDoctorsImprove = false;
    isArquebusImprove = false;

    configureTroop(soldier, ":/images/images/Military/infantry/farmer_stand_1.png", "soldier", 100,
                   1000, ":/images/images/Military/infantry/farmer_", 100, 100);
    configureTroop(archer, ":/images/images/Military/archer/archer_stand_1.png", "archer", 70,
                   1000, ":/images/images/Military/archer/archer_", 100, 100);
    configureTroop(rider, ":/images/images/Military/rider/cavalier_stand_1.png", "rider", 180,
                   1500, ":/images/images/Military/rider/cavalier_", 100, 100);
    configureTroop(mage, ":/images/images/Military/mag/mag_stand_1.png", "mage", 50,
                   500, ":/images/images/Military/mag/mag_", 100, 100);

    for(int i = 0; i < army.size(); i++)
        delete army[i];

    army.clear();
}

void Army::startAllTimers()
{
    for(int i = 0; i < army.size(); i++)
        army[i]->startAllTimers();
}

void Army::stopAllTimers()
{
    for(int i = 0; i < army.size(); i++)
        army[i]->stopAllTimers();
}

void Army::improveHauberk()
{
    if(!isHauberkImprove)
    {
        if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Hauberk"))
        {
            QString str = soldier->getImgPrefix();
            str.resize(str.size() - 7);
            str += "soldier_";
            soldier->setImgPrefix(str);
            soldier->setAttack(static_cast<int>(soldier->getAttack() + 30));
            soldier->setHp(soldier->getHp() + 250);
            soldier->setPixmap(QPixmap(soldier->getImgPrefix() + "stand_1.png"));

            for(int i = 0; i < army.size(); i++)
            {
                if(army[i]->getType() == "soldier")
                {
                    army[i]->setImgPrefix(str);
                    army[i]->setAttack(static_cast<int>(army[i]->getAttack() + 30));
                    army[i]->setHp(army[i]->getHp() + 250);
                }
            }
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Hauberk"));
            emit moneyWasted(parent->getPriceUpgrade("Hauberk"));
            emit modificate();
            isHauberkImprove = true;
            parent->deleteCurrentMenuItem();
        }
        else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
    }
}

void Army::improveArmor()
{
    if(isHauberkImprove && !isArmorImprove)
    {
        if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Armor"))
        {
            QString str = soldier->getImgPrefix();
            str.resize(str.size() - 8);
            str += "knight_";
            soldier->setImgPrefix(str);
            soldier->setAttack(static_cast<int>(soldier->getAttack() + 20));
            soldier->setHp(soldier->getHp() + 300);
            soldier->setPixmap(QPixmap(soldier->getImgPrefix() + "stand_1.png"));

            for(int i = 0; i < army.size(); i++)
            {
                if(army[i]->getType() == "soldier")
                {
                    army[i]->setImgPrefix(str);
                    army[i]->setAttack(static_cast<int>(army[i]->getAttack() + 20));
                    army[i]->setHp(army[i]->getHp() + 300);
                }
            }
            isArmorImprove = true;
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Armor"));
            emit moneyWasted(parent->getPriceUpgrade("Armor"));
            emit modificate();
            parent->deleteCurrentMenuItem();
        }
        else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
    }
}

void Army::improveWeapon()
{
    if(isArmorImprove && !isWeaponImprove)
    {
        if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Weapon"))
        {
            QString str = rider->getImgPrefix();
            str.resize(str.size() - 9);
            str += "rider_";
            rider->setImgPrefix(str);
            rider->setAttack(static_cast<int>(rider->getAttack() + 70));
            rider->setHp(rider->getHp() + 600);
            rider->setPixmap(QPixmap(rider->getImgPrefix() + "stand_1.png"));
            soldier->setAttack(static_cast<int>(soldier->getAttack() + 40));

            for(int i = 0; i < army.size(); i++)
            {
                if(army[i]->getType() == "rider")
                {
                    army[i]->setImgPrefix(str);
                    army[i]->setAttack(static_cast<int>(army[i]->getAttack() + 60));
                    army[i]->setHp(army[i]->getHp() + 600);
                }
                if(army[i]->getType() == "soldier")
                {
                    army[i]->setAttack(static_cast<int>(army[i]->getAttack() + 40));
                }
            }
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Weapon"));
            emit moneyWasted(parent->getPriceUpgrade("Weapon"));
            emit modificate();
            isWeaponImprove = true;
            parent->deleteCurrentMenuItem();
        }
        else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
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

        for(int i = 0; i < army.size(); i++)
        {
            army[i]->setHp(army[i]->getHp() + 50);
        }
        int price = parent->getPriceUpgrade("Quarantine");
        parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
        emit moneyWasted(parent->getPriceUpgrade("Quarantine"));
        emit modificate();
        parent->setPriceUpgrade("Quarantine", static_cast<int>(price * 1.6));
    }
    else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
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

            for(int i = 0; i < army.size(); i++)
            {
                army[i]->setHp(army[i]->getHp() * 2);
            }
            int price = parent->getPriceUpgrade("Doctors");
            parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
            emit moneyWasted(parent->getPriceUpgrade("Doctors"));
            emit modificate();
            isDoctorsImprove = true;
            parent->deleteCurrentMenuItem();
        }
        else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
    }
}

void Army::improveArquebus()
{
    if(!isArquebusImprove)
    {
        if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Arquebus"))
        {
            QString str = archer->getImgPrefix();
            str.resize(str.size() - 7);
            str += "crossbow_";
            archer->setImgPrefix(str);

            archer->setAttack(static_cast<int>(archer->getAttack() + 100));
            archer->setHp(archer->getHp() + 400);
            archer->setPixmap(QPixmap(archer->getImgPrefix() + "stand_1.png"));

            for(int i = 0; i < army.size(); i++)
            {
                if(army[i]->getType() == "archer")
                {
                    army[i]->setImgPrefix(str);
                    army[i]->setAttack(static_cast<int>(army[i]->getAttack() + 100));
                    army[i]->setHp(army[i]->getHp() + 400);
                }
            }
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Arquebus"));
            emit moneyWasted(parent->getPriceUpgrade("Arquebus"));
            emit modificate();
            isArquebusImprove = true;
            parent->deleteCurrentMenuItem();
        }
        else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
    }
}

void Army::increaseMageHitPoint()
{
    if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Mage_Hp"))
    {
        mage->setHp(mage->getHp() + 300);

        for(int i = 0; i < army.size(); i++)
        {
            if(army[i]->getType() == "mage")
                army[i]->setHp(army[i]->getHp() + 300);
        }
        int price = parent->getPriceUpgrade("Mage_Hp");
        parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
        emit moneyWasted(parent->getPriceUpgrade("Mage_Hp"));
        emit modificate();
        parent->setPriceUpgrade("Mage_Hp", static_cast<int>(price * 1.3));
    }
    else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
}

void Army::increaseMageAttack()
{
    if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Mage_Attack"))
    {
        mage->setAttack(static_cast<int>(mage->getAttack() + 30));

        for(int i = 0; i < army.size(); i++)
        {
            if(army[i]->getType() == "mage")
                army[i]->setAttack(static_cast<int>(army[i]->getAttack() + 30));
        }
        int price = parent->getPriceUpgrade("Mage_Attack");
        parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
        emit moneyWasted(parent->getPriceUpgrade("Mage_Attack"));
        emit modificate();
        parent->setPriceUpgrade("Mage_Attack", static_cast<int>(price * 1.3));
    }
    else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
}
