#include "constants.h"
#include "Military/troop.h"
#include <QFont>
#include <QTimer>


Troop::Troop()
{
    side = ConflictSide::Left;
    attack = 0;
    hp = 0;
    animationCounter = 1;
    timer_interval = 0;
    timer_remainingTime = 0;
    imgPrefix = "";
    type = "";
    status = Status::Stand;
    setPos(0, 0);
    hpText = new QGraphicsTextItem();
    hpText->setPos(0, 0);
}

Troop::~Troop()
{
    delete timer;
    delete hpText;
}

double Troop::getHp() const
{
    return this->hp;
}

int Troop::getAnimationCounter() const
{
    return animationCounter;
}

double Troop::getAttack() const
{
    return this->attack;
}

void Troop::setHp(const double & hp)
{
    this->hp = static_cast<int>(hp);
    hpText->setPlainText(QString::number(static_cast<int>(this->hp)));
}

void Troop::startAllTimers() const
{
    timer->start(timer_remainingTime);
}

void Troop::stopAllTimers()
{
    timer_remainingTime = timer->remainingTime();
    timer->stop();
}

void Troop::setSide(const ConflictSide & side)
{
    this->side = side;
    if(side == ConflictSide::Right)
    {
        QTransform t = makeTransform(this->boundingRect().size());
        this->setTransform(t);
    }
}

int Troop::getTimerInterval() const
{
    return this->timer_interval;
}

int Troop::getTimerRemainingTime() const
{
    return this->timer_remainingTime;
}

ConflictSide Troop::getSide() const
{
    return this->side;
}

Status Troop::getStatus() const
{
    return this->status;
}

void Troop::setStatus(const Status & status)
{
    if(this->status == status)
        return;
    this->status = status;
    animationCounter = 1;
}

void Troop::setType(const QString & type)
{
    this->type = type;
}

QString Troop::getType() const
{
    return this->type;
}

QString Troop::convertPrefixStatusAnimCounterToStr(const QString & imgPrefix, const Status & status, const int & animationCounter)
{
    if(status == Status::Attack)
        return QString(imgPrefix + "attack_" + QString::number(animationCounter) + ".png");
    if(status == Status::Run)
        return QString(imgPrefix + "run_" + QString::number(animationCounter) + ".png");
    else return QString(imgPrefix + "stand_1.png");
}

void Troop::setImgPrefix(const QString & imgPrefix)
{
    this->imgPrefix = imgPrefix;
}

QString Troop::getImgPrefix() const
{
    return this->imgPrefix;
}

QGraphicsTextItem * Troop::getTextItem() const
{
    return this->hpText;
}

void Troop::initialText()
{
    hpText->setPlainText(QString::number(static_cast<int>(this->hp)));
    hpText->setFont(QFont("Old English Text MT", 14));
    hpText->setDefaultTextColor(Qt::white);

    if(this->side == ConflictSide::Left)
        this->hpText->setPos(this->x() + this->pixmap().width() / 2, this->y() - TROOPS_HP_TEXT_HEIGHT_UNDER_TROOP);
    else this->hpText->setPos(this->x() + this->pixmap().width() / 3, this->y() - TROOPS_HP_TEXT_HEIGHT_UNDER_TROOP);
}

void Troop::setAttack(const int & attack)
{
    this->attack = attack;
}

void Troop::setTimerInterval(const int & time)
{
    this->timer_interval = time;
}

void Troop::setTimerRemainingTime(const int & time)
{
    this->timer_remainingTime = time;
}
