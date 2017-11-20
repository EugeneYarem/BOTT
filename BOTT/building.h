#ifndef BUILDING_H
#define BUILDING_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Building: public QObject
{
    Q_OBJECT
protected:
    QGraphicsPixmapItem*img;
    int x,y;
    int level;
public:
    Building();
    Building(QString file,int x,int y);
public slots:
    virtual void upgradeBld(){}//level++
};

#endif // BUILDING_H
