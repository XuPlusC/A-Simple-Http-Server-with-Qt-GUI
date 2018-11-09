#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ComNetWork_Lab_1_HttpServer.h"
#include <iostream>
#pragma execution_character_set("utf-8")

std::string Path;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Start_clicked()
{
    int Port = this->ui->lineEdit_Port->text().toInt();
    Path = this->ui->lineEdit_Path->text().toStdString();

//    std::thread t = std::thread(&StartServer, 8181);
//    t.detach();
     StartServer(Port);
}
