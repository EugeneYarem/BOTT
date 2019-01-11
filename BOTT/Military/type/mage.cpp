#include "mage.h"
#include <QTimer>


Mage::Mage()
{
    timer = new QTimer();
}

Mage::Mage(Troop & i)
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

    initialText();
    this->status = Status::Run;
    this->animationCounter = 1;
    this->imgPrefix = i.getImgPrefix();
    this->type = i.getType();
    this->timer_interval = i.getTimerInterval();
    this->timer_remainingTime = i.getTimerRemainingTime();

    timer = new QTimer();

    connect(timer, &QTimer::timeout, this, &Mage::animation);
    connect(timer, &QTimer::timeout, this, &Mage::run);
}

void Mage::animation()
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
        if(this->animationCounter > 6)
            this->animationCounter = 1;
    }
}

void Mage::run()
{
    timer->start(timer_interval);

    if(this->status != Status::Run)
        return;
    if(this->side == ConflictSide::Left)
    {
        this->setPos(this->x() + 5, this->y());
        this->hpText->setPos(this->x() + this->pixmap().width() / 4, this->y() - 30);

    }
    else
    {
        this->setPos(this->x() - 5, this->y());
        this->hpText->setPos(this->x() + this->pixmap().width() / 2, this->y() - 30);
    }
}
