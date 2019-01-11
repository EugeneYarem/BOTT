#ifndef INFANTRY_H
#define INFANTRY_H

#include "Military/troop.h"

class Infantry : public Troop
{

public:
    Infantry();
    Infantry(Troop & troop);

public slots:
    void animation() override;
    void run() override;

};

#endif // INFANTRY_H
