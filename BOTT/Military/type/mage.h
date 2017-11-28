#ifndef MAGE_H
#define MAGE_H

#include "Military/troop.h"

class Mage : public Troop
{

public:
    Mage();
    Mage(Troop &);

public slots:
    void Animation();
    void Run();
};

#endif // MAGE_H
