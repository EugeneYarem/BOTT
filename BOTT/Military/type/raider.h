#ifndef RAIDER_H
#define RAIDER_H

#include "Military/troop.h"

class Rider : public Troop
{

public:
    Rider();
    Rider(Troop &);

public slots:
    void Animation();
    void Run();
};

#endif // RAIDER_H
