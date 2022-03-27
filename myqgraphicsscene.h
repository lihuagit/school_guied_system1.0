#ifndef MYQGRAPHICSSCENE_H
#define MYQGRAPHICSSCENE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QMouseEvent>

class MyQGraphicsScene:public QGraphicsScene
{
    Q_OBJECT
public:
    MyQGraphicsScene(QWidget *parent = nullptr);
};

#endif // MYQGRAPHICSSCENE_H
