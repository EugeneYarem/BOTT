#include "infantry.h"
#include <QTimer>


Infantry::Infantry()
{
    timer = new QTimer();
}

Infantry::Infantry(Troop & i)
{
    this->setPixmap(i.pixmap());
    this->setTransform(i.transform());
    this->attack = i.getAttack();
    this->hp = i.getHp();
    this->side = i.getSide();
    if(this->side == ConflictSide::Left)
        this->setPos(300, 220);
    else
        this->setPos(2110, 220);

    initialText();
    this->status = Status::Run;
    this->animationCounter = 1;
    this->imgPrefix = i.getImgPrefix();
    this->timer_interval = i.getTimerInterval();
    this->timer_remainingTime = i.getTimerRemainingTime();
    this->type = i.getType();

    timer = new QTimer();

    connect(timer, &QTimer::timeout, this, &Infantry::animation);
    connect(timer, &QTimer::timeout, this, &Infantry::run);
}

void Infantry::animation()
{
    timer->start(timer_interval);

    if(this->status == Status::Stand)
        this->setPixmap(QPixmap(imgPrefix + "stand_1.png"));
    else if(this->status == Status::Run)
    {
        this->setPixmap(QPixmap(imgPrefix + "run_" + getNum(this->animationCounter) + ".png"));
        this->animationCounter++;
        if(this->animationCounter > 8)
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

void Infantry::run()
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
