#ifndef ARMY_H
#define ARMY_H

#include "enums.h"
#include "dataHandlers/jsonhandler.h"
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
    Army(View * parentView, const ConflictSide & side);
    ~Army();

    friend bool JsonHandler::convertJsonArrayToArmy(const QJsonArray & arr, Army * army) const;
    friend bool JsonHandler::convertJsonArrayToPrototypes(const QJsonArray & arr, Army * army) const;
    int getTownHp() const;
    int size() const;
    QVector<Troop *> getArmy() const;
    QVector<Troop *> getPrototypes() const;
    Troop * createEmptyTroop(const QString & type) const;
    Troop * getTroop(const int & i) const;
    void addRestoredTroopsOnScene(QGraphicsScene * scene);
    void addTroop(const QString & type, QGraphicsScene * scene);
    void clearStart();
    void deleteTroop();
    void setNewPrototype(const Troop * troop);
    void setTownHp(const int & hp);
    void startAllTimers() const;
    void stopAllTimers();

private:
    void configureTroop(Troop * troop, const QString & pixmapFileName, const QString & type, const int & attack, const int & hp, const QString & imgPref, const int & interval, const int & remainingTime);

public slots:
    void improveArmor();
    void improveArquebus();
    void improveDoctors();
    void improveHauberk();
    void increaseMageAttack();
    void increaseMageHitPoint();
    void improveQuarantine();
    void improveWeapon();
    void setArmorImprove();
    void setArquebusImprove();
    void setDoctorsImprove();
    void setHauberkImprove();
    void setWeaponImprove();

signals:
    void modificate(); // Сигнал, уведомляющий о том, что игрок купил улучшение.
    void moneyWasted(const int & sum); // Сигнал, указывающий на то, что были потрачены деньги. Кидать этот сигнал, при покупке улучшений
    void requiredShowMes(const QString & message);
    void uniteCreated(); // Сигнал, уведомляющий о создании юнита

};

#endif // ARMY_H
