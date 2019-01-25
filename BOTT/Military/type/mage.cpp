#include "constants.h"
#include "mage.h"
#include <QTimer>


Mage::Mage()
{
    timer = new QTimer();
}

Mage::Mage(const Troop & i)
{
    this->setPixmap(i.pixmap());
    this->setTransform(i.transform());
    this->attack = i.getAttack();
    this->hp = i.getHp();
    this->side = i.getSide();
    if(this->side == ConflictSide::Left)
        this->setPos(LEFT_START_POSITION_X, START_POSITION_Y);
    else
        this->setPos(RIGHT_START_POSITION_X, START_POSITION_Y);

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
        if(this->animationCounter > MAGE_RUN_ANIMATION_SLIDES_COUNT)
            this->animationCounter = 1;
        this->setPixmap(QPixmap(imgPrefix + "run_" + QString::number(this->animationCounter) + ".png"));
        this->animationCounter++;      
    }
    else if(this->status == Status::Attack)
    {
        if(this->animationCounter > MAGE_ATTACK_ANIMATION_SLIDES_COUNT)
            this->animationCounter = 1;
        this->setPixmap(QPixmap(imgPrefix + "attack_" + QString::number(this->animationCounter) + ".png"));
        this->animationCounter++;       
    }
}

void Mage::run()
{
    timer->start(timer_interval);

    if(this->status != Status::Run)
        return;
    if(this->side == ConflictSide::Left)
    {
        this->setPos(this->x() + TROOPS_STEP_WIDTH, this->y());
        this->hpText->setPos(this->x() + this->pixmap().width() / 4, this->y() - TROOPS_HP_TEXT_HEIGHT_UNDER_TROOP);
    }
    else
    {
        this->setPos(this->x() - TROOPS_STEP_WIDTH, this->y());
        this->hpText->setPos(this->x() + this->pixmap().width() / 2, this->y() - TROOPS_HP_TEXT_HEIGHT_UNDER_TROOP);
    }
}
