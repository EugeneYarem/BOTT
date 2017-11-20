#include "Military/army.h"

Army::Army()
{

}

void Army::addTroop(QString type)
{

}

troop *Army::getTroop (int n)
{
    if(n<arm.size())
    return arm[n];
    else return NULL;
}

int Army::size()
{
    return arm.size();
}
