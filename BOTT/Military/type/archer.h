#ifndef ARCHER_H
#define ARCHER_H

#include "Military/troop.h"

class Archer : public Troop
{

public:
    Archer();
    Archer(Troop &);

public slots:
    void Animation();
    void Run();
};

#endif // ARCHER_H
