#include <QPainter>

#include "constants.h"
#include "wall.h"

Wall::Wall(qreal x, qreal y)
{
    setPos(x, y);
    setData(GD_Type, GO_Wall);
}

QRectF Wall::boundingRect() const
{
    return QRectF(-TILE_SIZE,    -TILE_SIZE,
                   TILE_SIZE * 2, TILE_SIZE * 2 );
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->fillPath(shape(), Qt::black);
    painter->restore();
}

QPainterPath Wall::shape() const
{
    QPainterPath p;
    p.addRect(QRectF(0, 0, TILE_SIZE, TILE_SIZE));
    return p;
}
