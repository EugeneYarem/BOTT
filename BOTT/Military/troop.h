#ifndef TROOP_H
#define TROOP_H

#include <QGraphicsPixmapItem>
#include <QString>

class QTimer;

enum Status{stand, run, attack};

class Troop : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    // ЛЁША, ПОЖАЛУЙСТА, ИСПОЛЬЗУЙ ФУНКЦИИ (SET-еры, GET-еры) ДЛЯ ДОСТУПА К ПОЛЯМ КЛАССА ИЗ ДРУГИХ КЛАССОВ
    // НЕ ЗАБЫВАЙ ПРО ИНКАПСУЛЯЦИЮ

    float hp;//1-...
    float atack;//1-...
    float def;//0-100
    QString type;//type of the troop
    QString img_pref;//begin in the file's names of the troop's sprite list
    QTimer * timer;//timer for the animation
    int timer_interval;
    int timer_remainingTime;
    int amt_cnt;//counter for animation
    Status sts;//is run, stay,attack...

public:
    // Сейчас тут только те SET-еры, GET-еры, которые понадобились, чтобы откомпилировался код
    Troop();
    void stopAllTimers();
    void startAllTimers();
    float getHp();
    float getAtack();
    QString getType();
    void setHp(float);

public slots:
    virtual void Animation(){}
    virtual void Run(){}
};

#endif // TROOP_H
