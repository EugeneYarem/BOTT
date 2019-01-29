#include "constants.h"
#include "Military/army.h"
#include "Military/type/archer.h"
#include "Military/type/infantry.h"
#include "Military/type/mage.h"
#include "Military/type/raider.h"
#include "town.h"
#include "view.h"


Army::Army(View * parent, const ConflictSide & side)
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

void Army::addTroop(const QString & type, QGraphicsScene * scene)
{
    if(army.size() < army::MAX_ARMY_SIZE)
    {
        if(type == "soldier")
        {
            if(parent->getTown()->getMoney() >= army_units_price::SOLDIER_PRICE)
            {
                army.resize(army.size() + 1);
                army[army.size() - 1] = new Infantry(* soldier);
                army[army.size() - 1]->startAllTimers();
                scene->addItem(army[army.size() - 1]);
                scene->addItem(army[army.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - army_units_price::SOLDIER_PRICE);
                emit this->moneyWasted(army_units_price::SOLDIER_PRICE);
                emit this->uniteCreated();
            }
        }
        if(type == "archer")
        {
            if(parent->getTown()->getMoney() >= army_units_price::ARCHER_PRICE)
            {
                army.resize(army.size() + 1);
                army[army.size() - 1] = new Archer(* archer);
                army[army.size() - 1]->startAllTimers();
                scene->addItem(army[army.size() - 1]);
                scene->addItem(army[army.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - army_units_price::ARCHER_PRICE);
                emit this->moneyWasted(army_units_price::ARCHER_PRICE);
                emit this->uniteCreated();
            }
        }
        if(type == "rider")
        {
            if(parent->getTown()->getMoney() >= army_units_price::RIDER_PRICE)
            {
                army.resize(army.size() + 1);
                army[army.size() - 1] = new Rider(* rider);
                army[army.size() - 1]->startAllTimers();
                scene->addItem(army[army.size() - 1]);
                scene->addItem(army[army.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - army_units_price::RIDER_PRICE);
                emit this->moneyWasted(army_units_price::RIDER_PRICE);
                emit this->uniteCreated();
            }
        }
        if(type == "mage")
        {
            if(parent->getTown()->getMoney() >= army_units_price::MAGE_PRICE)
            {
                army.resize(army.size() + 1);
                army[army.size() - 1] = new Mage(* mage);
                army[army.size() - 1]->startAllTimers();
                scene->addItem(army[army.size() - 1]);
                scene->addItem(army[army.size() - 1]->getTextItem());
                parent->getTown()->setMoney(parent->getTown()->getMoney() - army_units_price::MAGE_PRICE);
                emit this->moneyWasted(army_units_price::MAGE_PRICE);
                emit this->uniteCreated();
            }
        }
    }
}

Troop * Army::getTroop(const int & n) const
{
    if(n < army.size())
        return army[n];
    else return nullptr;
}

void Army::addRestoredTroopsOnScene(QGraphicsScene * scene)
{
    foreach(Troop * i, army)
    {
        scene->addItem(i);
        scene->addItem(i->getTextItem());
    }
}

int Army::size() const
{
    return army.size();
}

QVector<Troop *> Army::getArmy() const
{
    return army;
}

QVector<Troop *> Army::getPrototypes() const
{
    QVector<Troop *> vec;
    vec.append(archer);
    vec.append(mage);
    vec.append(rider);
    vec.append(soldier);
    return vec;
}

Troop * Army::createEmptyTroop(const QString & type) const
{
    if(type == "soldier")
        return new Infantry(* soldier);
    if(type == "archer")
        return new Archer(* archer);
    if(type == "rider")
        return new Rider(* rider);
    if(type == "mage")
        return new Mage(* mage);
    return nullptr;
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

void Army::setNewPrototype(const Troop * t)
{
    if(t->getType() == "soldier")
        configureTroop(soldier, Troop::convertPrefixStatusAnimCounterToStr(t->getImgPrefix(), t->getStatus(), t->getAnimationCounter()), t->getType(),
                       static_cast<int>(t->getAttack()), static_cast<int>(t->getHp()), t->getImgPrefix(), troops_configuration::SOLDIER_INTERVAL, troops_configuration::SOLDIER_REMAINING_TIME);
    if(t->getType() == "archer")
        configureTroop(archer, Troop::convertPrefixStatusAnimCounterToStr(t->getImgPrefix(), t->getStatus(), t->getAnimationCounter()), t->getType(),
                       static_cast<int>(t->getAttack()), static_cast<int>(t->getHp()), t->getImgPrefix(), troops_configuration::ARCHER_INTERVAL, troops_configuration::ARCHER_REMAINING_TIME);
    if(t->getType() == "rider")
        configureTroop(rider, Troop::convertPrefixStatusAnimCounterToStr(t->getImgPrefix(), t->getStatus(), t->getAnimationCounter()), t->getType(),
                       static_cast<int>(t->getAttack()), static_cast<int>(t->getHp()), t->getImgPrefix(), troops_configuration::RIDER_INTERVAL, troops_configuration::RIDER_REMAINING_TIME);
    if(t->getType() == "mage")
        configureTroop(mage, Troop::convertPrefixStatusAnimCounterToStr(t->getImgPrefix(), t->getStatus(), t->getAnimationCounter()), t->getType(),
                       static_cast<int>(t->getAttack()), static_cast<int>(t->getHp()), t->getImgPrefix(), troops_configuration::MAGE_INTERVAL, troops_configuration::MAGE_REMAINING_TIME);
}

int Army::getTownHp() const
{
    return parent->getTown()->getHealth();
}

void Army::setTownHp(const int & hp)
{
    parent->getTown()->setHealth(hp);
}

void Army::configureTroop(Troop * troop, const QString & pixmapFileName, const QString & type, const int & attack, const int & hp, const QString & imgPref, const int & interval, const int & remainingTime)
{
    troop->setPixmap(QPixmap( pixmapFileName ));
    troop->setSide( this->side );
    troop->setType( type );
    troop->setAttack( attack );
    troop->setHp( hp );
    troop->setImgPrefix( imgPref );
    troop->setTimerInterval( interval );
    troop->setTimerRemainingTime( remainingTime );
}

void Army::clearStart()
{
    isArmorImprove = false;
    isHauberkImprove = false;
    isWeaponImprove = false;
    isDoctorsImprove = false;
    isArquebusImprove = false;

    configureTroop(soldier, ":/images/images/Military/infantry/farmer_stand_1.png", "soldier", troops_configuration::SOLDIER_ATTACK,
                   troops_configuration::SOLDIER_HP, ":/images/images/Military/infantry/farmer_", troops_configuration::SOLDIER_INTERVAL, troops_configuration::SOLDIER_REMAINING_TIME);
    configureTroop(archer, ":/images/images/Military/archer/archer_stand_1.png", "archer", troops_configuration::ARCHER_ATTACK,
                   troops_configuration::ARCHER_HP, ":/images/images/Military/archer/archer_", troops_configuration::ARCHER_INTERVAL, troops_configuration::ARCHER_REMAINING_TIME);
    configureTroop(rider, ":/images/images/Military/rider/cavalier_stand_1.png", "rider", troops_configuration::RIDER_ATTACK,
                   troops_configuration::RIDER_HP, ":/images/images/Military/rider/cavalier_", troops_configuration::RIDER_INTERVAL, troops_configuration::RIDER_REMAINING_TIME);
    configureTroop(mage, ":/images/images/Military/mag/mag_stand_1.png", "mage", troops_configuration::MAGE_ATTACK,
                   troops_configuration::MAGE_HP, ":/images/images/Military/mag/mag_", troops_configuration::MAGE_INTERVAL, troops_configuration::MAGE_REMAINING_TIME);

    for(int i = 0; i < army.size(); i++)
        delete army[i];

    army.clear();
}

void Army::startAllTimers() const
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
            soldier->setAttack(static_cast<int>(soldier->getAttack() + army_upgrade_coefficients::SOLDIER_ATTACK_UPGRADE_COEFFICIENT));
            soldier->setHp(soldier->getHp() + army_upgrade_coefficients::SOLDIER_HP_UPGRADE_COEFFICIENT);
            soldier->setPixmap(QPixmap(soldier->getImgPrefix() + "stand_1.png"));

            for(int i = 0; i < army.size(); i++)
            {
                if(army[i]->getType() == "soldier")
                {
                    army[i]->setImgPrefix(str);
                    army[i]->setAttack(static_cast<int>(army[i]->getAttack() + army_upgrade_coefficients::SOLDIER_ATTACK_UPGRADE_COEFFICIENT));
                    army[i]->setHp(army[i]->getHp() + army_upgrade_coefficients::SOLDIER_HP_UPGRADE_COEFFICIENT);
                }
            }
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Hauberk"));
            emit moneyWasted(parent->getPriceUpgrade("Hauberk"));
            emit modificate();
            isHauberkImprove = true;
            parent->deleteCurrentMenuItem();
            parent->removePriceUpgrade("Hauberk");
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
            soldier->setAttack(static_cast<int>(soldier->getAttack() + army_upgrade_coefficients::SOLDIER_ATTACK_UPGRADE2_COEFFICIENT));
            soldier->setHp(soldier->getHp() + army_upgrade_coefficients::SOLDIER_HP_UPGRADE2_COEFFICIENT);
            soldier->setPixmap(QPixmap(soldier->getImgPrefix() + "stand_1.png"));

            for(int i = 0; i < army.size(); i++)
            {
                if(army[i]->getType() == "soldier")
                {
                    army[i]->setImgPrefix(str);
                    army[i]->setAttack(static_cast<int>(army[i]->getAttack() + army_upgrade_coefficients::SOLDIER_ATTACK_UPGRADE2_COEFFICIENT));
                    army[i]->setHp(army[i]->getHp() + army_upgrade_coefficients::SOLDIER_HP_UPGRADE2_COEFFICIENT);
                }
            }
            isArmorImprove = true;
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Armor"));
            emit moneyWasted(parent->getPriceUpgrade("Armor"));
            emit modificate();
            parent->deleteCurrentMenuItem();
            parent->removePriceUpgrade("Armor");
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
            rider->setAttack(static_cast<int>(rider->getAttack() + army_upgrade_coefficients::RIDER_ATTACK_UPGRADE_COEFFICIENT));
            rider->setHp(rider->getHp() + army_upgrade_coefficients::RIDER_HP_UPGRADE_COEFFICIENT);
            rider->setPixmap(QPixmap(rider->getImgPrefix() + "stand_1.png"));
            soldier->setAttack(static_cast<int>(soldier->getAttack() + army_upgrade_coefficients::SOLDIER_ATTACK_UPGRADE3_COEFFICIENT));

            for(int i = 0; i < army.size(); i++)
            {
                if(army[i]->getType() == "rider")
                {
                    army[i]->setImgPrefix(str);
                    army[i]->setAttack(static_cast<int>(army[i]->getAttack() + army_upgrade_coefficients::RIDER_ATTACK_UPGRADE_COEFFICIENT));
                    army[i]->setHp(army[i]->getHp() + army_upgrade_coefficients::RIDER_HP_UPGRADE_COEFFICIENT);
                }
                if(army[i]->getType() == "soldier")
                {
                    army[i]->setAttack(static_cast<int>(army[i]->getAttack() + army_upgrade_coefficients::SOLDIER_ATTACK_UPGRADE3_COEFFICIENT));
                }
            }
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Weapon"));
            emit moneyWasted(parent->getPriceUpgrade("Weapon"));
            emit modificate();
            isWeaponImprove = true;
            parent->deleteCurrentMenuItem();
            parent->removePriceUpgrade("Weapon");
        }
        else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
    }
}

void Army::setArmorImprove()
{
    isArmorImprove = true;
}

void Army::setArquebusImprove()
{
    isArquebusImprove = true;
}

void Army::setDoctorsImprove()
{
    isDoctorsImprove = true;
}

void Army::setHauberkImprove()
{
    isHauberkImprove = true;
}

void Army::setWeaponImprove()
{
    isWeaponImprove = true;
}

void Army::improveQuarantine()
{
    if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Quarantine"))
    {
        soldier->setHp(soldier->getHp() + army_upgrade_coefficients::HP_QUARANTINE_UPGRADE_COEFFICIENT);
        rider->setHp(soldier->getHp() + army_upgrade_coefficients::HP_QUARANTINE_UPGRADE_COEFFICIENT);
        archer->setHp(soldier->getHp() + army_upgrade_coefficients::HP_QUARANTINE_UPGRADE_COEFFICIENT);
        mage->setHp(soldier->getHp() + army_upgrade_coefficients::HP_QUARANTINE_UPGRADE_COEFFICIENT);

        for(int i = 0; i < army.size(); i++)
        {
            army[i]->setHp(army[i]->getHp() + army_upgrade_coefficients::HP_QUARANTINE_UPGRADE_COEFFICIENT);
        }
        int price = parent->getPriceUpgrade("Quarantine");
        parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
        emit moneyWasted(parent->getPriceUpgrade("Quarantine"));
        emit modificate();
        parent->setPriceUpgrade("Quarantine", static_cast<int>(price * army_upgrade_coefficients::QUARANTINE_UPGRADE_NEW_PRICE_COEFFICIENT));
    }
    else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
}

void Army::improveDoctors()
{
    if(!isDoctorsImprove)
    {
        if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Doctors"))
        {
            soldier->setHp(soldier->getHp() * army_upgrade_coefficients::HP_DOCTORS_UPGRADE_COEFFICIENT);
            rider->setHp(soldier->getHp() * army_upgrade_coefficients::HP_DOCTORS_UPGRADE_COEFFICIENT);
            archer->setHp(soldier->getHp() * army_upgrade_coefficients::HP_DOCTORS_UPGRADE_COEFFICIENT);
            mage->setHp(soldier->getHp() * army_upgrade_coefficients::HP_DOCTORS_UPGRADE_COEFFICIENT);

            for(int i = 0; i < army.size(); i++)
            {
                army[i]->setHp(army[i]->getHp() * army_upgrade_coefficients::HP_DOCTORS_UPGRADE_COEFFICIENT);
            }
            int price = parent->getPriceUpgrade("Doctors");
            parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
            emit moneyWasted(parent->getPriceUpgrade("Doctors"));
            emit modificate();
            isDoctorsImprove = true;
            parent->deleteCurrentMenuItem();
            parent->removePriceUpgrade("Doctors");
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

            archer->setAttack(static_cast<int>(archer->getAttack() + army_upgrade_coefficients::ARCHER_ATTACK_UPGRADE_COEFFICIENT));
            archer->setHp(archer->getHp() + army_upgrade_coefficients::ARCHER_HP_UPGRADE_COEFFICIENT);
            archer->setPixmap(QPixmap(archer->getImgPrefix() + "stand_1.png"));

            for(int i = 0; i < army.size(); i++)
            {
                if(army[i]->getType() == "archer")
                {
                    army[i]->setImgPrefix(str);
                    army[i]->setAttack(static_cast<int>(army[i]->getAttack() + army_upgrade_coefficients::ARCHER_ATTACK_UPGRADE_COEFFICIENT));
                    army[i]->setHp(army[i]->getHp() + army_upgrade_coefficients::ARCHER_HP_UPGRADE_COEFFICIENT);
                }
            }
            parent->getTown()->setMoney(parent->getTown()->getMoney() - parent->getPriceUpgrade("Arquebus"));
            emit moneyWasted(parent->getPriceUpgrade("Arquebus"));
            emit modificate();
            isArquebusImprove = true;
            parent->deleteCurrentMenuItem();
            parent->removePriceUpgrade("Arquebus");
        }
        else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
    }
}

void Army::increaseMageHitPoint()
{
    if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Mage_Hp"))
    {
        mage->setHp(mage->getHp() + army_upgrade_coefficients::MAGE_HP_UPGRADE_COEFFICIENT);

        for(int i = 0; i < army.size(); i++)
        {
            if(army[i]->getType() == "mage")
                army[i]->setHp(army[i]->getHp() + army_upgrade_coefficients::MAGE_HP_UPGRADE_COEFFICIENT);
        }
        int price = parent->getPriceUpgrade("Mage_Hp");
        parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
        emit moneyWasted(parent->getPriceUpgrade("Mage_Hp"));
        emit modificate();
        parent->setPriceUpgrade("Mage_Hp", static_cast<int>(price * army_upgrade_coefficients::MAGE_HP_UPGRADE_NEW_PRICE_COEFFICIENT));
    }
    else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
}

void Army::increaseMageAttack()
{
    if(parent->getTown()->getMoney() >= parent->getPriceUpgrade("Mage_Attack"))
    {
        mage->setAttack(static_cast<int>(mage->getAttack() + army_upgrade_coefficients::MAGE_ATTACK_UPGRADE_COEFFICIENT));

        for(int i = 0; i < army.size(); i++)
        {
            if(army[i]->getType() == "mage")
                army[i]->setAttack(static_cast<int>(army[i]->getAttack() + army_upgrade_coefficients::MAGE_ATTACK_UPGRADE_COEFFICIENT));
        }
        int price = parent->getPriceUpgrade("Mage_Attack");
        parent->getTown()->setMoney(parent->getTown()->getMoney() - price);
        emit moneyWasted(parent->getPriceUpgrade("Mage_Attack"));
        emit modificate();
        parent->setPriceUpgrade("Mage_Attack", static_cast<int>(price * army_upgrade_coefficients::MAGE_ATTACK_UPGRADE_NEW_PRICE_COEFFICIENT));
    }
    else emit requiredShowMes("Недостаточно денег для покупки этого улучшения!");
}
