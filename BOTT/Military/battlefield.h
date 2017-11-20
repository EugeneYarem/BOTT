#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QObject>

class QTimer;
class Army;
class QGraphicsScene;

class Battlefield : public QObject
{
    Q_OBJECT

    QGraphicsScene * scene;
    QTimer * timer;
    Army * arm1, * arm2;

public:
    Battlefield();
    ~Battlefield();
    void setScene(QGraphicsScene *);

public slots:
    void Battle();

};

#endif // BATTLEFIELD_H
