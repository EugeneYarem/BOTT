#ifndef MAGE_H
#define MAGE_H

#include "Military/troop.h"

class Mage : public Troop
{

public:
    Mage();
    Mage(Troop & troop);

public slots:
    void animation() override;
    void run() override;

};

#endif // MAGE_H
