#include <QPainter>

#include "constants.h"
#include "food.h"

static const qreal FOOD_RADIUS = 8;
static const int interval = 13;
Food::Food(qreal x, qreal y,food_t z, GameController &controller):
    controller(controller)
{
    load = 0;
    coord_x = x;
    coord_y = y;
    cate = z;
    setPos(x, y);
    setData(GD_Type, GO_Food);
}
void Food::advance(int phase){
    if (!phase) return;
    if (cate != Limit) return;
    load++;
    if ((limitTime - load) % interval == 0)
        controller.changeTime((limitTime - load) / interval);
    if (load == limitTime)
        controller.countingDown();
}

QRectF Food::boundingRect() const
{
    return QRectF(-TILE_SIZE,    -TILE_SIZE,
                   TILE_SIZE * 2, TILE_SIZE * 2 );
}

void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    if (cate == Unlimit)
        painter->fillPath(shape(), Qt::red);
    else
        painter->fillPath(shape(), Qt::blue);

    painter->restore();
}

QPainterPath Food::shape() const
{
    QPainterPath p;
    p.addEllipse(QPointF(TILE_SIZE / 2, TILE_SIZE / 2), FOOD_RADIUS, FOOD_RADIUS);
    return p;
}
