#include "building.h"

Building::Building()
{

}
Building::Building(QString file, int x, int y)
{
    img = new QGraphicsPixmapItem();
    img->setPixmap(QPixmap(file));
    this->x = x;
    this->y = y;
    level = 0;
}
