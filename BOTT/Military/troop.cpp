#include "Military/troop.h"
#include <QTimer>

Troop::Troop()
{

}

double Troop::getHp()
{
    return hp;
}

double Troop::getAtack()
{
    return atack;
}

void Troop::setHp(float hp)
{
    this->hp = hp;
}

void Troop::startAllTimers()
{
    timer->start(timer_interval);
}

void Troop::stopAllTimers()
{
    timer->stop();
}

void Troop::setParty(PoC p)
{
    party=p;
    if(p==Right)
    {
        QTransform t = makeTransform( this->boundingRect().size() );
        this->setTransform( t );
    }
}

int Troop::getTime_interval()
{
    return this->timer_interval;
}

int Troop::getTime_remainingTime()
{
    return this->timer_remainingTime;
}

PoC Troop::getParty()
{
    return this->party;
}

Status Troop::getSts()
{
    return this->sts;
}

void Troop::setSts(Status sts)
{
    if(this->sts==sts)return;
    this->sts=sts;
    amt_cnt=1;
}

void Troop::setType(QString type)
{
    this->type=type;
}

QString Troop::getType()
{
    return this->type;
}

void Troop::setDef(int def)
{
    this->def=def;
}

void Troop::setImg_Pref(QString ip)
{
    this->img_pref=ip;
}

QString Troop::getImg_pref()
{
    return this->img_pref;
}

void Troop::setAtack(int atack)
{
    this->atack=atack;
}
