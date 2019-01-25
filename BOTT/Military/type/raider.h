#ifndef RAIDER_H
#define RAIDER_H

#include "Military/troop.h"

class Rider : public Troop
{

public:
    Rider();
    Rider(const Troop & troop);

public slots:
    void animation() override;
    void run() override;

};

#endif // RAIDER_H
