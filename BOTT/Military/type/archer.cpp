#include "archer.h"
#include "enums.h"
#include <QTimer>


Archer::Archer()
{
    timer = new QTimer();
}

Archer::Archer(Troop & i)
{
    this->setPixmap(i.pixmap());
    this->setTransform(i.transform());
    this->attack = i.getAttack();
    this->hp = i.getHp();
    this->side = i.getSide();
    if(this->side == ConflictSide::Left)
        this->setPos(300, 230);
    else
        this->setPos(2110, 230);

    this->initialText();
    this->status = Status::Run;
    this->animationCounter = 1;
    this->imgPrefix = i.getImgPrefix();

    this->timer_interval = i.getTimerInterval();
    this->timer_remainingTime = i.getTimerRemainingTime();

    this->type = i.getType();

    timer = new QTimer();

    connect(timer, &QTimer::timeout, this, &Archer::animation);
    connect(timer, &QTimer::timeout, this, &Archer::run);
}

void Archer::animation()
{
    timer->start(timer_interval);

    if(this->status == Status::Stand)
        this->setPixmap(QPixmap(imgPrefix + "stand_1.png"));
    else if(this->status == Status::Run)
    {
        this->setPixmap(QPixmap(imgPrefix + "run_" + getNum(this->animationCounter) + ".png"));
        this->animationCounter++;
        if(this->animationCounter > 7)
            this->animationCounter = 1;
    }
    else if(this->status == Status::Attack)
    {
        this->setPixmap(QPixmap(imgPrefix + "attack_" + getNum(this->animationCounter) + ".png"));
        this->animationCounter++;
        if(this->animationCounter > 7)
            this->animationCounter = 1;
    }
}

void Archer::run()
{
    timer->start(timer_interval);

    if(this->status != Status::Run)
        return;
    if(this->side == ConflictSide::Left)
    {
        this->setPos(this->x() + 5, this->y());
        this->hpText->setPos(this->x() + this->pixmap().width() / 2, this->y() - 30);

    }
    else
    {
        this->setPos(this->x() - 5, this->y());
        this->hpText->setPos(this->x() + this->pixmap().width() / 3, this->y() - 30);
    }

}
