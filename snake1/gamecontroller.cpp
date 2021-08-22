#include <QEvent>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QString>
#include <QMessageBox>
#include <QMediaPlayer>

#include "gamecontroller.h"
#include "food.h"
#include "snake.h"
#include "wall.h"
#include "constants.h"

GameController::GameController(QGraphicsScene &scene, QObject *parent) :
    QObject(parent),
    scene(scene),
    snake(NULL),
    limitFood(NULL),
    count(NULL),
    score(NULL),
    eat_food_num(0),
    game_score(0)
{
    timer.start( 1000/33 );
    addNewWall();
    addNewSnake();
    addNewFood();
    setScore();
    setTime();
    scene.installEventFilter(this);
    connect(&timer, SIGNAL(timeout()),&scene, SLOT(advance()));

}

GameController::~GameController()
{
}
void GameController::setScore(){
    score = new QGraphicsSimpleTextItem();
    score->setText("score");
    QFont font = score->font();
    font.setFamily("微软雅黑");
    font.setPixelSize(50);
    font.setPointSize(10);
    score->setFont(font);
    score->setBrush(QBrush(Qt::black));
    score->setPos(wide_edge_min, height_edge_max + TILE_SIZE);
    scene.addItem(score);
}

void GameController::setTime(){
    count = new QGraphicsSimpleTextItem();
    count->setText("time");
    QFont font = score->font();
    font.setFamily("微软雅黑");
    font.setPixelSize(50);
    font.setPointSize(10);
    count->setFont(font);
    count->setBrush(QBrush(Qt::black));
    count->setPos(wide_edge_max - TILE_SIZE * 6, height_edge_max + TILE_SIZE);
    scene.addItem(count);
}

void GameController::countingDown(){
    if (limitFood == NULL) return;
    scene.removeItem(limitFood);
    limitFood = NULL;
    return;
}

void GameController::snakeAteFood(Food *food)
{
    Food::food_t cate = food->cate;
    game_score++;
    changeScore();
    scene.removeItem(food);
    if (cate == Food::Unlimit){
        eat_food_num++;
        if (eat_food_num == 5){
            eat_food_num = 0;
            if (limitFood == NULL) addLimitFood();
        }
        addNewFood();
    } else {
        limitFood = NULL;
        changeTime(0);
    }


}

void GameController::changeScore(){
    QString str;
    str.setNum(game_score);
    QString ss = "score ";
    ss.append(str);
    score->setText(ss);
}

void GameController::changeTime(int t){
    QString str;
    str.setNum(t);
    QString ss = "time ";
    ss.append(str);
    count->setText(ss);
}

void GameController::snakeHitWall(Snake *snake, Wall *wall){
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

void GameController::snakeAteItself(){
    QTimer::singleShot(0, this, SLOT(gameOver()));
}


void GameController::listen_key_board(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Left:
            snake->change_direction(Snake::MoveLeft);
            break;
        case Qt::Key_Right:
            snake->change_direction(Snake::MoveRight);
            break;
        case Qt::Key_Up:
            snake->change_direction(Snake::MoveUp);
            break;
        case Qt::Key_Down:
            snake->change_direction(Snake::MoveDown);
            break;
        case Qt::Key_1:
            snake->change_direction(Snake::MoveFast);
            break;
        case Qt::Key_2:
            snake->change_direction(Snake::MoveSlow);
            break;
        }
}

void GameController::addNewFood(){
    int x = ((int)(qrand() % 200) / 18 - 9) * 20;
    int y = ((int)(qrand() % 200) / 18 - 9) * 20;

    while (snake->shape().contains(snake->mapFromScene(QPointF(x + 5, y + 5))) ||
           (limitFood != NULL && x == limitFood->coord_x && y == limitFood->coord_y)){
        x = ((int)(qrand() % 200) / 18 - 9) * 20;
        y = ((int)(qrand() % 200) / 18 - 9) * 20;
    }

    Food *food = new Food(x, y, Food::Unlimit, *this);
    unlimitfood = food;
    scene.addItem(food);
}

void GameController::addLimitFood(){
    int x = ((int)(qrand() % 200) / 18 - 9) * 20;
    int y = ((int)(qrand() % 200) / 18 - 9) * 20;

    while (snake->shape().contains(snake->mapFromScene(QPointF(x + 5, y + 5))) ||
           (unlimitfood != NULL && x == unlimitfood->coord_x && y == unlimitfood->coord_y)){
        x = ((int)(qrand() % 200) / 18 - 9) * 20;
        y = ((int)(qrand() % 200) / 18 - 9) * 20;
    }

    Food *food = new Food(x, y, Food::Limit, *this);
    limitFood = food;
    scene.addItem(food);
}

void GameController::addNewSnake(){
    snake = new Snake(*this);
    scene.addItem(snake);
}

void GameController::addNewWall(){
    Wall *wall = NULL;
    for (int i = -10; i < 10; i++){
        wall = new Wall(i * 20, wide_edge_min);
        scene.addItem(wall);
        wall = new Wall(i * 20, wide_edge_max - TILE_SIZE);
        scene.addItem(wall);
        wall = new Wall(height_edge_min, i * 20);
        scene.addItem(wall);
        wall = new Wall(height_edge_max - TILE_SIZE, i * 20);
        scene.addItem(wall);
    }

}

void GameController::gameOver()
{
    disconnect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
    if (QMessageBox::Yes == QMessageBox::information(NULL, tr("Game Over"), tr("Again?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)) {
        connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
        scene.clear();
        limitFood = NULL;
        unlimitfood = NULL;
        game_score = 0;
        eat_food_num = 0;
        addNewSnake();
        addNewFood();
        addNewWall();
        setScore();
        setTime();
    } else {
        exit(0);
    }
}

bool GameController::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        listen_key_board((QKeyEvent *)event);
        return true;
    } else {
        return QObject::eventFilter(object, event);
    }
}
