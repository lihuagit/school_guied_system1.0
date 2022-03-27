#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->add_edge_btu,&QPushButton::clicked,(ui->graphicsView),&MyQGraphicsView::AddEdge);
    connect(ui->add_pos_btu,&QPushButton::clicked,(ui->graphicsView),&MyQGraphicsView::AddEPos);
    connect(ui->show_poss_btu,&QPushButton::clicked,(ui->graphicsView),&MyQGraphicsView::show_poss);
    connect(ui->show_edges_btu,&QPushButton::clicked,(ui->graphicsView),&MyQGraphicsView::show_edges);
    connect(ui->show_places_btu,&QPushButton::clicked,(ui->graphicsView),&MyQGraphicsView::show_places);
    connect(ui->clear_btu,&QPushButton::clicked,(ui->graphicsView),&MyQGraphicsView::Clear);
    connect(ui->add_place_btu,&QPushButton::clicked,this,&MainWindow::AddPlace);
    connect(ui->erase_place_btu,&QPushButton::clicked,this,&MainWindow::ErasePlace);
    connect(ui->query_btu,&QPushButton::clicked,this,&MainWindow::query_path);
    connect(ui->key_btu,&QPushButton::clicked,this,&MainWindow::check_key);
    ui->buttonGroup->setId(ui->short_btu,1);
    ui->buttonGroup->setId(ui->far_btu,2);
    // 隐藏开发者功能按钮，待到密码输入正确
    ui->add_edge_btu->hide();
    ui->add_pos_btu->hide();
    ui->show_poss_btu->hide();
    ui->show_edges_btu->hide();
    ui->show_places_btu->hide();
    ui->clear_btu->hide();

    // 为box控件添加已有地点名称
    updata_box();

    // 设置box属性
    ui->snameBox->setEditable(true);
    ui->enameBox->setEditable(true);
    ui->name_Box->setEditable(true);
    ui->snameBox->lineEdit()->setPlaceholderText("请输入起点名称");
    ui->enameBox->lineEdit()->setPlaceholderText("请输入终点名称");
    ui->name_Box->lineEdit()->setPlaceholderText("请输入地点名称");
    ui->snameBox->lineEdit()->clear();
    ui->enameBox->lineEdit()->clear();
    ui->name_Box->lineEdit()->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updata_box(){
    ui->snameBox->clear();
    ui->enameBox->clear();
    ui->name_Box->clear();
    std::map<std::string, Edge>::iterator it;
    for(it=ui->graphicsView->mapMap.m_place.begin();it!=ui->graphicsView->mapMap.m_place.end();it++){
        ui->snameBox->addItem(QString(it->first.data()));
        ui->enameBox->addItem(QString(it->first.data()));
        ui->name_Box->addItem(QString(it->first.data()));
    }
}

void MainWindow::updata_info_label(){
    double len=ui->graphicsView->mapMap.ans_len;
    double k=ui->graphicsView->mapMap.ans_k;
    QString slen=QString::number(len)+"m";
    QString sk=QString::number(k)+"J";
    ui->len_label->setText(slen);
    ui->k_label->setText(sk);
}
void MainWindow::AddPlace(){
    std::string name;
    name=ui->name_Box->currentText().toStdString();
    if(name.size()==0){
        QMessageBox::critical(this,"错误","请输入待添加地点名称！！！");
        return ;
    }
    if(ui->graphicsView->mapMap.m_place.find(name)!=ui->graphicsView->mapMap.m_place.end()){
        QMessageBox::critical(this,"错误","添加地点名称已存在");
        return ;
    }
    if( ui->graphicsView->AddPlace(name) ==true )
        QMessageBox::information(this,"提示","添加新地点成功！！！");
    ui->snameBox->addItem(QString(name.data()));
    ui->enameBox->addItem(QString(name.data()));
    ui->name_Box->addItem(QString(name.data()));
}

void MainWindow::ErasePlace(){
    std::string name;
    name=ui->name_Box->currentText().toStdString();
    if(name.size()==0){
        QMessageBox::critical(this,"错误","请输入待删除地点名称！！！");
        return ;
    }
    if(ui->graphicsView->mapMap.m_place.find(name)==ui->graphicsView->mapMap.m_place.end()){
        QMessageBox::critical(this,"错误","待删除地点名称不存在！！！");
        return ;
    }
    if( ui->graphicsView->ErasePlace(name) == false){
        QMessageBox::critical(this,"错误","打开文件错误！！！");
        return ;
    }
    QMessageBox::information(this,"提示","删除地点成功！！！");
    updata_box();
}

void MainWindow::query_path(){
    std::string sname=ui->snameBox->currentText().toStdString();
    std::string ename=ui->enameBox->currentText().toStdString();
    if(sname.size()==0){
        QMessageBox::critical(this,"错误","请输入起点地点名称！！！");
        return ;
    }
    if(ename.size()==0){
        QMessageBox::critical(this,"错误","请输入终点地点名称！！！");
        return ;
    }
    if(sname==ename){
        QMessageBox::critical(this,"错误","起点终点相同！！！");
        return ;
    }
    if(ui->graphicsView->mapMap.m_place.find(sname)==ui->graphicsView->mapMap.m_place.end()){
        QMessageBox::critical(this,"错误","起点不存在！！！");
        return ;
    }
    if(ui->graphicsView->mapMap.m_place.find(ename)==ui->graphicsView->mapMap.m_place.end()){
        QMessageBox::critical(this,"错误","终点不存在！！！");
        return ;
    }
    int state=ui->buttonGroup->checkedId();
    if( ui->graphicsView->query_path(sname,ename,state) == false )
        QMessageBox::critical(this,"错误","未选择查询状态或不存在路径！！！");
    updata_info_label();
}

void MainWindow::check_key(){
    static bool is_ok=false;
    std::string key=ui->key_Edit->text().toStdString();
    if(is_ok){
        QMessageBox::information(this,"提示","您已经处于开发者状态！！！");
        return ;
    }
    if(key.size()==0){
        QMessageBox::critical(this,"错误","请输入密码以使用开发者功能！！！");
        return ;
    }
    if(key!="root"){
        QMessageBox::critical(this,"错误","密码错误！！！");
        return ;
    }
    is_ok=true;
    ui->key_label->setText("欢迎使用开发者功能！！！");
    ui->key_label->setFont(QFont("", 11, QFont::Bold));
    ui->key_label->setAlignment(Qt::AlignCenter);
    ui->add_edge_btu->show();
    ui->add_pos_btu->show();
    ui->show_poss_btu->show();
    ui->show_edges_btu->show();
    ui->show_places_btu->show();
    ui->clear_btu->show();
}