#ifndef INFANTRY_H
#define INFANTRY_H

#include "Military/troop.h"

class Infantry : public Troop
{

public:
    Infantry();
    Infantry(Troop &);

public slots:
    void Animation();
    void Run();
};

#endif // INFANTRY_H
