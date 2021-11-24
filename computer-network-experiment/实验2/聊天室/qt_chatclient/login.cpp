#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include <QtDebug>
#include <string>

//登陆窗口对象的构造函数
login::login(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    pa=parent;
    ui->setupUi(this);
    std::pair<std::string,std::string> res;
    connect(ui->pushButton,&QPushButton::clicked,[&](){
        res.first = ui->lineEdit->text().toUtf8().data();
        res.second  = ui->lineEdit_2->text().toUtf8().data();
        pa->list = res;
        this->close();
    });
}

login::~login()
{
    delete ui;
}
