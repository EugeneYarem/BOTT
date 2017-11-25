#ifndef ARMY_H
#define ARMY_H

#include <QObject>
#include <QVector>
#include "Military/troop.h"

class View;
class QGraphicsScene;

class Army: public QObject
{
    Q_OBJECT

    View * parent; // Указатель на view. Нужен, чтобы из армии получить доступ к данным из класса Town
    QVector<Troop *> arm;
    Troop wizard, soldier, archer, rider;//for upgrade

public:
    Army(View * );
    void addTroop(QString type, QGraphicsScene * scene);
    Troop * getTroop(int n);
    int size();

signals:
    void moneyWasted(int); // Сигнал, указывающий на то, что были потрачены деньги. Кидать этот сигнал, при покупке улучшений
    void uniteCreated(); // Сигнал, уведомляющий о создании юнита
    void modificate(); // Сигнал, уведомляющий о том, что игрок купил улучшение. Кидать этот сигнал во всех слотах, которые отвечают за улучшения в ответ на выбор пунктов игрового меню

};

#endif // ARMY_H
