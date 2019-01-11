#ifndef ARMY_H
#define ARMY_H

#include "enums.h"
#include "Military/troop.h"
#include <QObject>

class Troop;
class View;

class Army : public QObject
{
    Q_OBJECT

    bool isArmorImprove;
    bool isArquebusImprove;
    bool isDoctorsImprove;
    bool isHauberkImprove;
    bool isWeaponImprove;
    ConflictSide side;
    QVector<Troop *> army;
    Troop * archer, * mage, * rider, * soldier; // for upgrade
    View * parent; // Указатель на view. Нужен, чтобы из армии получить доступ к данным из класса Town

public:
    Army(View * parentView, ConflictSide side);
    ~Army();

    int getTownHp();
    int size();
    Troop * getTroop(int i);
    void addTroop(QString type, QGraphicsScene * scene);
    void clearStart();
    void deleteTroop();
    void setTownHp(int hp);
    void startAllTimers();
    void stopAllTimers();

private:
    void configureTroop(Troop * troop, QString pixmapFileName, QString type, int attack, int hp, QString imgPref, int interval, int remainingTime);

public slots:
    void improveArmor();
    void improveArquebus();
    void improveDoctors();
    void improveHauberk();
    void increaseMageAttack();
    void increaseMageHitPoint();
    void improveQuarantine();
    void improveWeapon();

signals:
    void modificate(); // Сигнал, уведомляющий о том, что игрок купил улучшение.
    void moneyWasted(int sum); // Сигнал, указывающий на то, что были потрачены деньги. Кидать этот сигнал, при покупке улучшений
    void requiredShowMes(QString message);
    void uniteCreated(); // Сигнал, уведомляющий о создании юнита

};

#endif // ARMY_H
