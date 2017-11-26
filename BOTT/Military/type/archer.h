#ifndef ARCHER_H
#define ARCHER_H


#include <QObject>
#include "Military/troop.h"
#include "Military/army.h"

class Army;

class Archer:public Troop
{
public:
    Archer();
    Archer(Troop &);
public slots:
    void Animation();
    void Run();
};

#endif // ARCHER_H
