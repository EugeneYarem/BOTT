#ifndef ARMY_H
#define ARMY_H

#include <QObject>
#include <QVector>
#include "Military/troop.h"

class QGraphicsScene;

class Army: public QObject
{
    Q_OBJECT

    QVector<troop *> arm;
    troop wizard, soldier, archer, rider;//for upgrade

public:
    Army();
    void addTroop(QString type, QGraphicsScene * scene);
    troop * getTroop(int n);
    int size();

};

#endif // ARMY_H
