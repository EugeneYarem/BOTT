#ifndef RECT_H
#define RECT_H

#include <QGraphicsRectItem>

class Rect : public QGraphicsRectItem
{
public:
    Rect();

    // QGraphicsItem interface
protected:
    void keyPressEvent(QKeyEvent *event);

    // QGraphicsItem interface
protected:
    bool sceneEvent(QEvent *event);
};

#endif // RECT_H
