#ifndef ARMY_H
#define ARMY_H

#include <QObject>
#include <QVector>
#include "Military/troop.h"

class View;
class Troop;
class QGraphicsScene;

class Army : public QObject
{
    Q_OBJECT

    View * parent; // Указатель на view. Нужен, чтобы из армии получить доступ к данным из класса Town
    QVector<Troop *> arm;
    Troop * mage , * soldier, * archer, * rider;//for upgrade
    PoC party;
    bool isArmorImprove;
    bool isHauberkImprove;
    bool isWeaponImprove;
    bool isDoctorsImprove;
    bool isArquebusImprove;

public:
    Army(View * , PoC party);
    ~Army();
    void addTroop(QString type, QGraphicsScene * scene);
    Troop * getTroop(int n);
    int size();
    void setParty(PoC);
    void deleteTroop();
    int getTownHp();
    void setTownHp(int hp);

public slots:
    void improveHauberk();
    void improveArmor();
    void improveWeapon();
    void improveQuarantine();
    void improveDoctors();
    void improveArquebus();
    void increaseMageHitPoint();
    void increaseMageAttack();

signals:
    void moneyWasted(int); // Сигнал, указывающий на то, что были потрачены деньги. Кидать этот сигнал, при покупке улучшений
    void uniteCreated(); // Сигнал, уведомляющий о создании юнита
    void modificate(); // Сигнал, уведомляющий о том, что игрок купил улучшение.
};

#endif // ARMY_H
