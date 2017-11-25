#include "Military/army.h"
#include "view.h"
#include "town.h"
#include <QGraphicsScene>

Army::Army(View * parent)
{
    this->parent = parent;
}

void Army::addTroop(QString type, QGraphicsScene * scene)
{
    emit uniteCreated();
}

Troop * Army::getTroop(int n)
{
    if(n < arm.size())
        return arm[n];
    else return NULL;
}

int Army::size()
{
    return arm.size();
}
