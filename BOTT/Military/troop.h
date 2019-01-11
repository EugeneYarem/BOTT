#ifndef TROOP_H
#define TROOP_H

#include "enums.h"
#include "Military/transform.h"
#include <QGraphicsPixmapItem>

class Troop : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

protected:
    ConflictSide side;
    double attack;
    double hp;
    int animationCounter; // counter for animation
    int timer_interval;
    int timer_remainingTime;
    QGraphicsTextItem * hpText;
    QString imgPrefix;// begin in the file's names of the troop's sprite list
    QString type;
    QTimer * timer; // timer for the animation
    Status status; // it run, stay, attack

public:
    Troop();
    ~Troop();

    ConflictSide getSide();
    double getAttack();
    double getHp();
    int getTimerInterval();
    int getTimerRemainingTime();
    QGraphicsTextItem * getTextItem();
    QString getImgPrefix();
    QString getType();
    Status getStatus();
    void initialText();
    void setAttack(int attack);
    void setHp(double hp);
    void setImgPrefix(QString imgPrefix);
    void setTimerInterval(int time);
    void setTimerRemainingTime(int time);
    void setType(QString type);
    void setSide(ConflictSide side);
    void setStatus(Status status);
    void startAllTimers();
    void stopAllTimers();

public slots:
    virtual void animation() = 0;
    virtual void run() = 0;

};

#endif // TROOP_H
