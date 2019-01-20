#include "transform.h"


QTransform makeTransform(const QSizeF & objSize)
{
    QTransform res;
    qreal fDx = 1., fDy = 1.;
    QPointF ptOffset;
    fDx = -1;
    ptOffset.setX(-objSize.width());
    res.scale(fDx, fDy);
    res.translate(ptOffset.x(), ptOffset.y());
    return res;
}
