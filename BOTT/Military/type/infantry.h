#ifndef INFANTRY_H
#define INFANTRY_H

#include <QObject>
#include "Military/troop.h"
#include "Military/army.h"

class Army;

class Infantry:public Troop
{
public:
    Infantry();
    Infantry(Troop &);
public slots:
    void Animation();
    void Run();
};

#endif // INFANTRY_H
