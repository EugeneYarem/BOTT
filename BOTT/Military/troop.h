#ifndef TROOP_H
#define TROOP_H

#include <QGraphicsPixmapItem>
#include <QString>
#include <QTimer>
#include "Military/transform.h"

enum Status{stand, run, attack};
enum PoC{Left,Right};//сторона конфликта

class Troop : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    // ЛЁША, ПОЖАЛУЙСТА, ИСПОЛЬЗУЙ ФУНКЦИИ (SET-еры, GET-еры) ДЛЯ ДОСТУПА К ПОЛЯМ КЛАССА ИЗ ДРУГИХ КЛАССОВ
    // НЕ ЗАБЫВАЙ ПРО ИНКАПСУЛЯЦИЮ
protected:
    double hp;//1-...
    double atack;//1-...
    double def;//0-100
    QString img_pref;//begin in the file's names of the troop's sprite list
    QTimer * timer;//timer for the animation
    int timer_interval;
    int timer_remainingTime;
    int amt_cnt;//counter for animation
    Status sts;//is run, stay,attack...
    PoC party;
    QString type;
public:

    Troop();
    double getHp();
    double getAtack();
    void setHp(float);
    void startAllTimers();
    void stopAllTimers();
    void setParty(PoC);
    int getTime_interval();
    int getTime_remainingTime();
    PoC getParty();
    Status getSts();
    void setSts(Status);
    void setType(QString);
    QString getType();
    void setAtack(int atack);
    void setDef(int def);
    void setImg_Pref(QString);
    QString getImg_pref();

public slots:
    virtual void Animation()=0;
    virtual void Run()=0;
};

#endif // TROOP_H
