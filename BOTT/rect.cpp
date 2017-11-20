#include "rect.h"
#include <QKeyEvent>
#include <QDebug>

Rect::Rect()
{
    this->setRect(0, 0, 100, 100);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

void Rect::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << "Rect react";
    if(event->nativeVirtualKey() == Qt::Key_A)
        this->setPos(this->pos().x() - 10, this->pos().y());
    if(event->nativeVirtualKey() == Qt::Key_D)
        this->setPos(this->pos().x() + 10, this->pos().y());
}

bool Rect::sceneEvent(QEvent *event)
{
    //qDebug() << "Rect sceneEvent";
    //qDebug() << event->type();
    if (event->type() == QEvent::KeyRelease)
    {
        keyPressEvent((QKeyEvent *)event);
        return true;
    }
    else return false;
}
