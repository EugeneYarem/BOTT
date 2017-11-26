#ifndef TOWN_H
#define TOWN_H

#include <QObject>
#include <QGraphicsPixmapItem>

class QTimer;
class QGraphicsTextItem;

class Town : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    int health;
    int money;
    int income; // доход от шахты
    QTimer * incomeTimer;
    int incomeTimer_interval;
    int incomeTimer_remainingTime;
    QGraphicsTextItem * healthItem;
    QGraphicsTextItem * moneyItem;

signals:
    void moneyEarned(int); // Сигнал, указывающий на то, что шахта принесла прибыль
    void moneyWasted(int); // Сигнал, указывающий на то, что были потрачены деньги на улучшение шахты

public:
    Town();
    int getHealth();
    int getMoney();
    void stopAllTimers();
    void startAllTimers();
    void addHealthMoneyToScene();
    void setHealth(int);
    void setMoney(int);

public slots:
    void damage(int );
    void addMoney();
    void setNewIncome();

};

#endif // TOWN_H
