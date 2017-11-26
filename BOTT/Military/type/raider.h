#ifndef RAIDER_H
#define RAIDER_H

#include <QObject>
#include "Military/troop.h"
#include "Military/army.h"

class Army;

class Rider:public Troop
{
public:
    Rider();
    Rider(Troop &);
public slots:
    void Animation();
    void Run();
};

#endif // RAIDER_H
