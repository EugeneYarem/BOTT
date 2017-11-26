#include "raider.h"

Rider::Rider()
{

}

Rider::Rider(Troop &i)
{
    this->setPixmap(i.pixmap());
    this->setTransform(i.transform());
    this->atack=i.getAtack();
    this->hp=i.getHp();
    this->def=0;
    this->party=i.getParty();
    if(this->party==Left)
        this->setPos(300,230);
    else
        this->setPos(2110,230);
    sts=run;
    amt_cnt=1;
    this->img_pref=i.getImg_pref();
    this->type=i.getType();
    //this->timer_interval = i.getTime_interval();
   // this->timer_remainingTime = i.getTime_remainingTime();
    timer_interval = 100;
    timer_remainingTime = 100;
    timer=new QTimer();

    connect(timer,SIGNAL(timeout()),this,SLOT(Animation()));
    connect(timer,SIGNAL(timeout()),this,SLOT(Run()));

}

void Rider::Animation()
{
    if(sts==stand)
    {
        this->setPixmap(QPixmap(img_pref+"stand_1.png"));
    }
    else if(sts==run)
    {

        this->setPixmap(QPixmap(img_pref+"run_"+getNum(amt_cnt)+".png"));
        amt_cnt++;
        if(amt_cnt>8)amt_cnt=1;
    }
    else if(sts==attack)
    {
        this->setPixmap(QPixmap(img_pref+"attack_"+getNum(amt_cnt)+".png"));
        amt_cnt++;
        if(amt_cnt>4)amt_cnt=1;
    }
}

void Rider::Run()
{
    if(sts!=run)return;
    if(this->party==Left)
    {
    this->setPos(this->x()+5,this->y());
    }
    else
    {
    this->setPos(this->x()-5,this->y());
    }
}
