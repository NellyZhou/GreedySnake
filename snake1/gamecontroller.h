#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QGraphicsItem>

class QGraphicsScene;
class QKeyEvent;

class Snake;
class Food;
class Wall;

class GameController : public QObject
{
    Q_OBJECT
public:
    GameController(QGraphicsScene &scene, QObject *parent = 0);
    ~GameController();

    void snakeAteFood(Food *food);                  //处理蛇吃食物的信息
    void snakeHitWall(Snake *snake, Wall *wall);    //处理触碰障碍物的信息
    void snakeAteItself();                          //处理贪吃蛇碰到自己的信息
    void countingDown();                            //处理限时食物计时的信息
    void changeTime(int t);                         //处理限时食物计时的信息

public slots:
    void gameOver();

protected:
     bool eventFilter(QObject *object, QEvent *event);

private:
    void listen_key_board(QKeyEvent *event);        //监听键盘
    void addNewFood();                              
    void addNewSnake();
    void addNewWall();                  
    void addLimitFood();                            //产生新的item
    void setScore();
    void setTime();
    void changeScore();                             //改变游戏信息输出


    int eat_food_num;
    int game_score;
    QTimer timer;                                   //通过timer与各个item的计时链接
    QGraphicsScene &scene;
    Snake *snake;
    Food *limitFood;
    Food *unlimitfood;
    QGraphicsSimpleTextItem *score;
    QGraphicsSimpleTextItem *count;
};

#endif // GAMECONTROLLER_H
