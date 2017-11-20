#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QObject>
#include <QTimer>
#include "Military/army.h"
#include <cmath>

class Battlefield:public QObject
{
    Q_OBJECT
    QTimer*timer;
    Army *arm1,*arm2;
public:
    Battlefield();
    ~Battlefield();

public slots:
    void Battle();
};

#endif // BATTLEFIELD_H
