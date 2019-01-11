#include "Military/troop.h"
#include <QFont>
#include <QTimer>


Troop::Troop()
{
}

Troop::~Troop()
{
    delete timer;
    delete hpText;
}

double Troop::getHp()
{
    return this->hp;
}

double Troop::getAttack()
{
    return this->attack;
}

void Troop::setHp(double hp)
{
    this->hp = static_cast<int>(hp);
    hpText->setPlainText(QString::number(static_cast<int>(this->hp)));
}

void Troop::startAllTimers()
{
    timer->start(timer_remainingTime);
}

void Troop::stopAllTimers()
{
    timer_remainingTime=timer->remainingTime();
    timer->stop();
}

void Troop::setSide(ConflictSide side)
{
    this->side = side;
    if(side == ConflictSide::Right)
    {
        QTransform t = makeTransform(this->boundingRect().size());
        this->setTransform(t);
    }
}

int Troop::getTimerInterval()
{
    return this->timer_interval;
}

int Troop::getTimerRemainingTime()
{
    return this->timer_remainingTime;
}

ConflictSide Troop::getSide()
{
    return this->side;
}

Status Troop::getStatus()
{
    return this->status;
}

void Troop::setStatus(Status status)
{
    if(this->status == status)
        return;
    this->status = status;
    animationCounter = 1;
}

void Troop::setType(QString type)
{
    this->type = type;
}

QString Troop::getType()
{
    return this->type;
}

void Troop::setImgPrefix(QString imgPrefix)
{
    this->imgPrefix = imgPrefix;
}

QString Troop::getImgPrefix()
{
    return this->imgPrefix;
}

QGraphicsTextItem * Troop::getTextItem()
{
    return this->hpText;
}

void Troop::initialText()
{
    hpText = new QGraphicsTextItem();

    if(this->side == ConflictSide::Left)
    {
        hpText->setPos(this->x() - 20, 190);
    }
    else
    {
        hpText->setPos(this->x() + 20, 190);
    }

    hpText->setPlainText(QString::number(static_cast<int>(this->hp)));
    hpText->setFont(QFont("Old English Text MT", 14));
    hpText->setDefaultTextColor(Qt::white);

    if(this->side == ConflictSide::Left)
        this->hpText->setPos(this->x() + this->pixmap().width() / 2, this->y() - 35);
    else this->hpText->setPos(this->x() + this->pixmap().width() / 3, this->y() - 30);
}

void Troop::setAttack(int attack)
{
    this->attack = attack;
}

void Troop::setTimerInterval(int time)
{
    this->timer_interval = time;
}

void Troop::setTimerRemainingTime(int time)
{
    this->timer_remainingTime = time;
}
