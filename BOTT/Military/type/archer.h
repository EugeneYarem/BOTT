#ifndef ARCHER_H
#define ARCHER_H

#include "Military/troop.h"

class Archer : public Troop
{

public:
    Archer();
    Archer(const Troop & troop);

public slots:
    void animation() override;
    void run() override;

};

#endif // ARCHER_H
