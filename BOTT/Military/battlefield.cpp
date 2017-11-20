#include "Military/battlefield.h"

Battlefield::Battlefield()
{
    timer = new QTimer();
    arm1=new Army();
    arm2=new Army();
    connect(timer,SIGNAL(timeout()),this,SLOT(Battle()));//каждую секунду проверяем есть ли войска рядом
    timer->start(1000);//и каждую секунду бьем противника, если он близко
}

Battlefield::~Battlefield()
{
    delete arm1;
    delete arm2;
}

void Battlefield::Battle()
{
    if(arm1->size()==0||arm2->size()==0)return;//Заглушка, пока нет атаки на город

    if(arm2->getTroop(0)->x()-arm1->getTroop(0)->x()<=50)//если они достаточно близко, то расчитываем показатели атаки
    {


   float dmg1=0,dmg2=0;
   for(int i=0;i<arm1->size();i++)
   {
       if(arm1->getTroop(i)->type=="raider"&&arm2->getTroop(0)->type=="archer")//Двойной урон от "противоположного" типа войск
       {
           dmg1+=arm1->getTroop(i)->atack*std::pow(0.5,i)*2;
       }
       else if(arm1->getTroop(i)->type=="archer"&&arm2->getTroop(0)->type=="soldier")
       {
           dmg1+=arm1->getTroop(i)->atack*std::pow(0.5,i)*2;
       }
       else if(arm1->getTroop(i)->type=="soldier"&&arm2->getTroop(0)->type=="raider")
       {
           dmg1+=arm1->getTroop(i)->atack*std::pow(0.5,i)*2;
       }
       else
       dmg1+=arm1->getTroop(i)->atack*std::pow(0.5,i);
   }
   for(int i=0;i<arm2->size();i++)
   {
       if(arm2->getTroop(i)->type=="raider"&&arm1->getTroop(0)->type=="archer")
       {
           dmg2+=arm2->getTroop(i)->atack*std::pow(0.5,i)*2;
       }
       else if(arm2->getTroop(i)->type=="archer"&&arm1->getTroop(0)->type=="soldier")
       {
           dmg2+=arm2->getTroop(i)->atack*std::pow(0.5,i)*2;
       }
       else if(arm2->getTroop(i)->type=="soldier"&&arm1->getTroop(0)->type=="raider")
       {
           dmg2+=arm2->getTroop(i)->atack*std::pow(0.5,i)*2;
       }
       else
       dmg2+=arm2->getTroop(i)->atack*std::pow(0.5,i);
   }
  // arm1->getTroop(0)->hp-=dmg2;
   //arm2->getTroop(0)->hp-=dmg1;//тут должно быть удаление солдат из армии и так далее
//...
   }
}