#include "Military/army.h"
#include <QGraphicsScene>

Army::Army()
{

}

void Army::addTroop(QString type, QGraphicsScene * scene)
{

}

troop *Army::getTroop(int n)
{
    if(n < arm.size())
        return arm[n];
    else return NULL;
}

int Army::size()
{
    return arm.size();
}
