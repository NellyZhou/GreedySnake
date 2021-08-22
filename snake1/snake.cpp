#include <QPainter>

#include "constants.h"
#include "gamecontroller.h"
#include "snake.h"

static const qreal SNAKE_SIZE = TILE_SIZE;

Snake::Snake(GameController &controller) :
    isAilve(true),
    head(0, 0),
    is_eat_food(1),
    speed(5),

    m_direction(NoMove),
    controller(controller)

{
}

QRectF Snake::boundingRect() const
{
    qreal xmin = head.x();
    qreal xmax = head.x();
    qreal ymin = head.y();
    qreal ymax = head.y();

    foreach (QPointF p, tail) {
        if (p.x() > xmax) xmax = p.x();
        if (p.x() < xmin) xmin = p.x();
        if (p.y() > ymax) ymax = p.y();
        if (p.y() < ymin) ymin = p.y();
    }

    QPointF a = mapFromScene(QPointF(xmin, ymin));
    QPointF b = mapFromScene(QPointF(xmax, ymax));

    QRectF bound = QRectF(a.x(), a.y(), b.x() - a.x() + SNAKE_SIZE, b.y() - a.y() + SNAKE_SIZE);
    return bound;
}

QPainterPath Snake::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(QRectF(0, 0, SNAKE_SIZE, SNAKE_SIZE));
    foreach (QPointF p, tail) {
        QPointF itemp = mapFromScene(p);
        path.addRect(QRectF(itemp.x(), itemp.y(), SNAKE_SIZE, SNAKE_SIZE));
    }
    return path;
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    if (isAilve)
        painter->fillPath(shape(), Qt::yellow);
    else
        painter->fillPath(shape(), Qt::black);
    painter->restore();
}

void Snake::change_direction(Direction direction)
{
    switch (direction){
        case MoveUp:
            if (m_direction == MoveDown) break;
            m_direction = MoveUp;
            break;
        case MoveLeft:
            if (m_direction == MoveRight) break;
            m_direction = MoveLeft;
            break;
        case MoveDown:
            if (m_direction == MoveUp) break;
            m_direction = MoveDown;
            break;
        case MoveRight:
            if (m_direction == MoveLeft) break;
            m_direction = MoveRight;
            break;
        case MoveFast:
            moveFast();
            break;
        case MoveSlow:
            moveSlow();
            break;
    }
}


void Snake::advance(int step){
    if (!step) return;
    load++;
    if (load % speed !=0 || m_direction == NoMove) return;

    if (is_eat_food > 0) {
        QPointF tailPoint = head;
        tail << tailPoint;
        is_eat_food--;
    } else {
        tail.removeFirst();
        tail << head;
    }

    switch (m_direction) {
        case MoveLeft:
            moveLeft();
            break;
        case MoveRight:
            moveRight();
            break;
        case MoveUp:
            moveUp();
            break;
        case MoveDown:
            moveDown();
            break;
    }

    setPos(head);
    state_of_snake();
}

void Snake::moveLeft(){
    head.rx() -= SNAKE_SIZE;
    if (head.rx() < -200) {
        head.rx() = 180;
    }
}

void Snake::moveRight(){
    head.rx() += SNAKE_SIZE;
    if (head.rx() >= 200) {
        head.rx() = -200;
    }
}

void Snake::moveUp()
{
    head.ry() -= SNAKE_SIZE;
    if (head.ry() < -200) {
        head.ry() = 180;
    }
}

void Snake::moveDown()
{
    head.ry() += SNAKE_SIZE;
    if (head.ry() >= 200) {
        head.ry() = -200;
    }
}

void Snake::moveFast(){
    speed--;
    if (speed == 0) speed++;
}

void Snake::moveSlow(){
    speed++;
}

void Snake::state_of_snake()
{
    QList<QGraphicsItem *> collisions = collidingItems();

    foreach (QGraphicsItem *food, collisions) {
        if (food->data(GD_Type) == GO_Food) {
            controller.snakeAteFood((Food *)food);
            is_eat_food += 1;
        }
        if (food->data(GD_Type) == GO_Wall){
            isAilve = false;
            controller.snakeHitWall(this, (Wall *)food);
            break;
        }
    }

    if (tail.contains(head)) {
        isAilve = false;
        controller.snakeAteItself();
    }
}
