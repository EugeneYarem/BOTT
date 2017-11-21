#ifndef TOWN_H
#define TOWN_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Town : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    int health;
public:
    Town();
    int getHealth();

public slots:
    void damage(int );

};

#endif // TOWN_H
