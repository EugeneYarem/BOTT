#ifndef TROOP_H
#define TROOP_H

#include <QGraphicsPixmapItem>
#include <QString>

class QTimer;

enum Status{stand, run, attack};

class troop : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    float hp;//1-...
    float atack;//1-...
    float def;//0-100
    QString type;//type of the troop
    QString img_pref;//begin in the file's names of the troop's sprite list
    QTimer * timer;//timer for the animation
    int amt_cnt;//counter for animation
    Status sts;//is run, stay,attack...
    troop();

public slots:
    virtual void Animation(){}
    virtual void Run(){}
};

#endif // TROOP_H
