#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QFont>
#include <QLineEdit>
#include <bits/stdc++.h>
#include "map.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void AddPlace();
    void ErasePlace();
    void query_path();
    void check_key();
    void updata_box();
    void updata_info_label();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
