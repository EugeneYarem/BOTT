#ifndef TOWN_H
#define TOWN_H

#include "keeper.h"
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

    friend bool Keeper::convertJsonObjectToTown(const QJsonObject & obj, Town * town) const;
    friend void Keeper::convertTownToJsonObject(const Town * town, QJsonObject & obj) const;
    int getHealth() const;
    int getMoney() const;
    void addHealthMoneyToScene();
    void clearStart();
    void setHealth(const int & hp);
    void setMoney(const int & money);
    void startAllTimers() const;
    void stopAllTimers();

public slots:
    void addMoney();
    void damage(const int & damage);
    void setNewIncome();

signals:
    void loose();
    void modificate(); // Сигнал, уведомляющий о том, что игрок купил улучшение.
    void moneyEarned(const int & income); // Сигнал, указывающий на то, что шахта принесла прибыль
    void moneyWasted(const int & wasted); // Сигнал, указывающий на то, что были потрачены деньги на улучшение шахты
    void requiredShowMes(const QString & text);

};

#endif // TOWN_H
