#include "myqgraphicsview.h"
#include <QDebug>

MyQGraphicsView::MyQGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
    scene=new MyQGraphicsScene;
    mapPix=new QPixmap("./src/hugong_map.jpg");
//    mapPix=new QPixmap("./hugong_map.jpg");
//    if(mapPix->isNull()) mapPix=new QPixmap("../hugong_map.jpg");
    scene->setSceneRect(0,0,mapPix->width(),mapPix->height());
    scene->addPixmap(*mapPix);
    this->setScene(scene);
    setDragMode(ScrollHandDrag);


    pen.setColor(QColor(0, 160, 230));
    pen.setWidth(5);

    QString info;
    info+="一个菜鸡";

    QGraphicsTextItem * text = new QGraphicsTextItem();
    text->setPlainText("李华");
    text->setPos(50,50);
    text->setFont(QFont("consolas",4,1));
    text->setToolTip(info);
    this->scene->addItem(text);

    Item=NULL;
    Start=Pos();
    End=Pos();
}
MyQGraphicsView::~MyQGraphicsView(){

}

void MyQGraphicsView::wheelEvent(QWheelEvent *event){
    // 获取鼠标滚轮的距离
    int wheelDeltaValue = event->delta();

    // 向上滚动，放大
    if (wheelDeltaValue > 0)
    {
        this->scale(1.2, 1.2);
    }
    // 向下滚动，缩小
    else
    {
        this->scale(1.0 / 1.2, 1.0 / 1.2);
    }
}
void MyQGraphicsView::mousePressEvent(QMouseEvent *event){
    // 如果是鼠标左键按下   改变指针形状，并且存储当前指针位置与窗口位置的差值。
    if(true)
        if(event->button() == Qt::LeftButton){
            // qDebug()<<"MyQGraphicsView";
            QPoint MapPos=event->pos();
            // qDebug()<<event->x();
            // qDebug()<<event->y();
            scencePos=mapToScene(MapPos);    // 获取scence坐标
            // qDebug()<<"scence pos:";
            // qDebug()<<scencePos.x();
            // qDebug()<<scencePos.y();
            // qDebug()<<endl;
            
            // if(Item!=NULL){
            //     scene->removeItem(Item);
            //     delete Item;
            //     Item=NULL;
            // }
            Clear();
            nowPos=Pos(scencePos.x(),scencePos.y());
            if(Start!=Pos() && End!=Pos()){
                Clear();
                
                Start=Pos();
                End=Pos();
            }
            if(false){
                Pos temp(scencePos.x(),scencePos.y());
                if(mapMap.m_pos.find(temp)!=mapMap.m_pos.end()){
                    typename std::set<Pos, pos_cmp>::iterator itr;
                    itr=mapMap.m_pos.find(temp);
                    Item=new QGraphicsEllipseItem((*itr).x,(*itr).y,5,5);
                    Item->setPen(pen);
                    Item->setFlags(QGraphicsItem::ItemIsSelectable);
                    scene->addItem(Item);
                    // Item=NULL;
                    if(Start==Pos()){
                        Start=(*itr);
                        // End=Pos();
                    }else{
                        End=(*itr);
                    }
                }
            }else{
                Item=new QGraphicsEllipseItem(scencePos.x(),scencePos.y(),5,5);
                Item->setPen(pen);
                scene->addItem(Item);
            }
        }
    QGraphicsView::mousePressEvent(event);
}

void MyQGraphicsView::AddEdge(){
    mapMap.AddEdge(Start,End);
    QGraphicsLineItem* temp=new QGraphicsLineItem();
    temp->setPen(pen);
    temp->setLine(QLineF(Start.x, Start.y, End.x, End.y));
    scene->addItem(temp);
    // qDebug()<<"AddEdge()";
}

void MyQGraphicsView::AddEPos(){
    if( mapMap.AddEPos(scencePos.x(),scencePos.y()) ) Item=NULL;
    // qDebug()<<"AddEPos()";
}

bool MyQGraphicsView::AddPlace(std::string name){
    // qDebug()<<"AddPlace()";
    if(nowPos==Pos(0,0)) return false;
    return mapMap.AddPlace(name,nowPos);
}

bool MyQGraphicsView::ErasePlace(std::string name){
    return mapMap.ErasePlace(name);
}

void MyQGraphicsView::Clear(){
    scene->clear();
    scene->addPixmap(*mapPix);
}

void MyQGraphicsView::show_poss(){
    Clear();
    std::map<Pos,std::vector<Edge>>::iterator it;
    for(it=mapMap.m_pos_edge.begin();it!=mapMap.m_pos_edge.end();++it){
        Item=new QGraphicsEllipseItem(it->first.x,it->first.y,5,5);
        Item->setPen(pen);
        Item->setFlags(QGraphicsItem::ItemIsSelectable);
        scene->addItem(Item);
    }
}

void MyQGraphicsView::show_edges(){
    Clear();
    for(auto temp:mapMap.m_edge){
        QGraphicsLineItem* tempp=new QGraphicsLineItem();
        tempp->setPen(pen);
        tempp->setLine(QLineF(temp.start_pos.x, temp.start_pos.y, temp.end_pos.x, temp.end_pos.y));
        scene->addItem(tempp);
    }
}

void MyQGraphicsView::show_places(){
    Clear();
    for(auto temp:mapMap.m_place){
        QGraphicsLineItem* tempp=new QGraphicsLineItem();
        tempp->setPen(pen);
        tempp->setLine(QLineF(temp.second.start_pos.x, temp.second.start_pos.y, temp.second.end_pos.x, temp.second.end_pos.y));
        scene->addItem(tempp);
    }
}
void MyQGraphicsView::solve_epos(){
    std::set<Edge>::iterator it;
    std::vector<Pos> temp;
    for(it=mapMap.m_edge.begin();it!=mapMap.m_edge.end();it++){
        temp.push_back(it->start_pos);
        temp.push_back(it->end_pos);
    }
    int len=temp.size();
    for(int i=0;i<len;i++){
        for(int j=i+1;j<len;j++){
            if( temp[i]==temp[j] ) temp[j]=temp[i];
        }
    }
    mapMap.m_edge.clear();
    for(int i=0;i<len;i+=2)
        mapMap.m_edge.insert(Edge(temp[i],temp[i+1]));
    
    std::ofstream fout;
    fout.open("./src/edge_temp.txt",std::ios::out);
    // if(fout.bad()) fout.open("edge_temp.txt",std::ios::out);
    if(fout.good()){
        int x1,y1,x2,y2;
        for(auto temp:mapMap.m_edge){
            x1=temp.start_pos.x;
            y1=temp.start_pos.y;
            x2=temp.end_pos.x;
            y2=temp.end_pos.y;
            fout<<x1<<" "<<y1<<" ";
            fout<<x2<<" "<<y2<<std::endl;
        }
        fout.close();
    }
}

void MyQGraphicsView::solve_pos(){
    std::map<Pos,std::vector<Edge>>::iterator it;
    std::ofstream fout;
    fout.open("./src/edge_pos_temp.txt",std::ios::out);
    
    for(it=mapMap.m_pos_edge.begin();it!=mapMap.m_pos_edge.end();++it){
        int x,y;
        x=it->first.x;
        y=it->first.y;
        fout<<x<<" "<<y<<std::endl;
    }
    fout.close();
}

void MyQGraphicsView::show_edge(Pos s,Pos e){
    QGraphicsLineItem* temp=new QGraphicsLineItem();
    temp->setPen(pen);
    temp->setLine(QLineF(s.x, s.y, e.x, e.y));
    scene->addItem(temp);
}
bool MyQGraphicsView::query_path(std::string sname,std::string ename,int state){
    std::vector<Pos> ans;
    // state==1表示最短路径状态
    // state==2表示最远路径状态
    if(state==1)
        ans=mapMap.dijkstra(sname,ename);
    else if(state==2)
        ans=mapMap.best_far(sname,ename);
    else false;
    int len=ans.size();
    if(len==0) return false;
    Clear();
    show_edge(mapMap.m_place[sname].start_pos,mapMap.m_place[sname].end_pos);
    for(int i=1;i<len;i++)
        show_edge(ans[i],ans[i-1]);
    show_edge(mapMap.m_place[ename].start_pos,mapMap.m_place[ename].end_pos);
    return true;
}
