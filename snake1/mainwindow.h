#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QGraphicsView;

class GameController;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initScene();
    void initSceneBackground();
    void adjustViewSize();


    QGraphicsScene *scene;
    QGraphicsView *view;
    GameController *game;
};

#endif // MAINWINDOW_H
