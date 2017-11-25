#ifndef ARMY_H
#define ARMY_H

#include <QObject>
#include <QVector>
#include "Military/troop.h"

class QGraphicsScene;

class Army: public QObject
{
    Q_OBJECT

    QVector<Troop *> arm;
    Troop wizard, soldier, archer, rider;//for upgrade

public:
    Army();
    void addTroop(QString type, QGraphicsScene * scene);
    Troop * getTroop(int n);
    int size();

};

#endif // ARMY_H
