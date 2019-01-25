#ifndef TROOP_H
#define TROOP_H

#include "enums.h"
#include "keeper.h"
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

    ConflictSide getSide() const;
    double getAttack() const;
    double getHp() const;
    friend bool Keeper::convertJsonObjectToTroop(const QJsonObject & obj, Troop * troop) const;
    friend void Keeper::convertArmyToJsonArray(const QVector<Troop *> & army, QJsonArray & arr) const;   
    int getAnimationCounter() const;
    int getTimerInterval() const;
    int getTimerRemainingTime() const;
    QGraphicsTextItem * getTextItem() const;
    QString getImgPrefix() const;
    QString getType() const;
    static QString convertPrefixStatusAnimCounterToStr(const QString & imgPrefix, const Status & status, const int & animationCounter = 1);
    Status getStatus() const;
    void initialText();
    void setAttack(const int & attack);
    void setHp(const double & hp);
    void setImgPrefix(const QString & imgPrefix);
    void setTimerInterval(const int & time);
    void setTimerRemainingTime(const int & time);
    void setType(const QString & type);
    void setSide(const ConflictSide & side);
    void setStatus(const Status & status);
    void startAllTimers() const;
    void stopAllTimers();

public slots:
    virtual void animation() = 0;
    virtual void run() = 0;

};

#endif // TROOP_H
