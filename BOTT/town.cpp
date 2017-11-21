#include "town.h"

Town::Town() : QObject()
{
    health = 100;

    setPixmap(QPixmap(":images/images/towns/town_1.png"));
}

int Town::getHealth()
{
    return health;
}

void Town::damage(int damage)
{
    health -= damage;
}
