#ifndef FOOD_H
#define FOOD_H

#include <QGraphicsItem>
#include "gamecontroller.h"


class Food : public QGraphicsItem
{
protected:
    int load;                               //食物缓冲计时
    static const int limitTime = 150;       //限时食物的时限
    void advance(int phase);                
public:
    qreal coord_x;
    qreal coord_y;
    enum food_t {
        Limit,
        Unlimit
    };

    food_t cate;                            //食物种类

    Food(qreal x, qreal y,food_t z, GameController &controller);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    QPainterPath shape() const;
private:
    GameController &controller;
};

#endif
