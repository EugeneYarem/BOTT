#ifndef TOWN_H
#define TOWN_H

#include <QGraphicsPixmapItem>
#include <QObject>

class View;

class Town : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    int health;
    int income; // доход от шахты
    int incomeTimer_interval;
    int incomeTimer_remainingTime;
    int money;
    QGraphicsTextItem * healthItem;
    QGraphicsTextItem * moneyItem;
    QTimer * incomeTimer;
    View * parent;

public:
    Town(View * parent);
    ~Town();

    int getHealth();
    int getMoney();
    void addHealthMoneyToScene();
    void clearStart();
    void setHealth(int hp);
    void setMoney(int money);
    void startAllTimers();
    void stopAllTimers();

public slots:
    void addMoney();
    void damage(int damage);
    void setNewIncome();

signals:
    void loose();
    void modificate(); // Сигнал, уведомляющий о том, что игрок купил улучшение.
    void moneyEarned(int income); // Сигнал, указывающий на то, что шахта принесла прибыль
    void moneyWasted(int wasted); // Сигнал, указывающий на то, что были потрачены деньги на улучшение шахты
    void requiredShowMes(QString text);

};

#endif // TOWN_H
