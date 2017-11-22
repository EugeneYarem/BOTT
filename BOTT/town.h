#ifndef TOWN_H
#define TOWN_H

#include <QObject>
#include <QGraphicsPixmapItem>

class QTimer;

class Town : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    int health;
    int money;
    int income; // доход от шахты
    QTimer * incomeTimer;

public:
    Town();
    int getHealth();
    int getMoney();

public slots:
    void damage(int );
    void addMoney();
    void setNewIncome();

};

#endif // TOWN_H
