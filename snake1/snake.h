#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QRectF>

class GameController;

class Snake : public QGraphicsItem
{
public:
    enum Direction {
        NoMove,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        MoveFast,
        MoveSlow
    };

    Snake(GameController & controller);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void change_direction(Direction direction); //改变移动


protected:
    void advance(int step);

private:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();        //贪吃蛇前进方向的控制
    void moveFast();
    void moveSlow();        //贪吃蛇前进速度的调节

    void state_of_snake();  //贪吃蛇状态的判断

    bool           isAilve;
    QPointF        head;            //蛇头
    int            is_eat_food;
    int            speed;           //速度（缓冲量大小）
    QList<QPointF> tail;            //蛇身
    int            load;            //前进的当前缓冲
    Direction      m_direction;     //当前前进方向
    GameController &controller;     //MVC中控制器
};

#endif // SNAKE_H
