#ifndef MAGE_H
#define MAGE_H


#include <QObject>
#include "Military/troop.h"
#include "Military/army.h"

class Army;

class Mage:public Troop
{
public:
    Mage();
    Mage(Troop &);
public slots:
    void Animation();
    void Run();
};

#endif // MAGE_H
