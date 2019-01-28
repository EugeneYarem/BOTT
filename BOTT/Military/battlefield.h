#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QObject>

class Army;
class QGraphicsScene;
class QTimer;

class Battlefield : public QObject
{
    Q_OBJECT

    Army * arm1, * arm2;
    int timer_interval;
    int timer_remainingTime;
    int timerB_interval;
    int timerB_remainingTime;
    QGraphicsScene * scene;
    QTimer * timer, * timer_B;

public:
    Battlefield();
    ~Battlefield();

    QGraphicsScene * getScene() const;
    void clearStart();
    void setArmies(Army * army1, Army * army2);
    void setScene(QGraphicsScene * scene);
    void startAllTimers() const;
    void stopAllTimers();

private:
    double countArmyAttack(Army * arm1, Army * arm2) const;

public slots:
    void armyControl() const;
    void battle() const;

};

#endif // BATTLEFIELD_H
