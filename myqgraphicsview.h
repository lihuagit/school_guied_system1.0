#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QApplication>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QString>
#include "myqgraphicsscene.h"
#include "map.h"

class MyQGraphicsView:public QGraphicsView
{
    Q_OBJECT
public:
    MyQGraphicsView(QWidget *parent = nullptr);
    ~MyQGraphicsView();
private:
    MyQGraphicsScene* scene;
    QPixmap* mapPix;
    QGraphicsEllipseItem* Item;
private:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event);        //单击
public:
    Map mapMap;
    QPointF scencePos;
    QPen pen;   // 定义一个画笔，设置画笔颜色和宽度
    Pos Start;
    Pos End;
    Pos nowPos;
public:
    void AddEdge();
    void AddEPos();
    bool AddPlace(std::string name);
    bool ErasePlace(std::string name);
    void Clear();
    void solve_epos();
    void solve_pos();
    void show_poss();
    void show_edges();
    void show_places();
    void show_edge(Pos s,Pos e);
    bool query_path(std::string sname,std::string ename,int state);
};

#endif // MYQGRAPHICSVIEW_H