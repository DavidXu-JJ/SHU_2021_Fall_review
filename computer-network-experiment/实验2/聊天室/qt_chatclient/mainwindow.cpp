#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mythread.h"
#include <string.h>
#include <iostream>
#include <QThread>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <QTimer>
#include <login.h>
#include <QMessageBox>
#include <QtDebug>


#define PORT 2333
#define maxnum 1024
#define getLen(zero) sizeof(zero)/sizeof(zero[0])

//显示聊天内容
void MainWindow::display(QString str){
    ui->textBrowser->append(str);
}

//发送单条信息
void MainWindow::send_message(const int &sockfd, std::string s){
    if(send(sockfd,s.c_str(),s.size(),0)==-1){
        perror("send()");
        ::close(sockfd);
        this->close();
    }
}

//接收单条信息
std::string MainWindow::recv_message(const int &sockfd){
    char message[maxnum];
    int num=recv(sockfd,message,maxnum,0);
    if(num==-1){
        perror("recv()");
        ::close(sockfd);
        this->close();
    }
    message[num]='\0';
    std::string res(message);
    return res;
}

//创建主窗口对象的构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    int numbytes;
    struct sockaddr_in their_addr;

    if((this->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            perror("socket():");
            this->close();
        }

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr.s_addr = INADDR_ANY;

    memset(&their_addr.sin_zero, 0, sizeof(their_addr.sin_zero));

    //连接服务器
    if(::connect(this->fd,(struct sockaddr *)&their_addr,sizeof(struct sockaddr)) == -1) {
        perror("connect():");
        this->close();
    }

    ui->setupUi(this);

    this->show();

    if(!this->SLOP_openNewWidget()){
        exit(0);
    }
    recvThread * recv = new recvThread(this);

    //关联主窗口的“离开按钮”与退出事件，点击按钮后可以退出
    connect(ui->pushButton_2,&QPushButton::clicked,[=](){
        ::close(this->fd);
        this->close();
        recv->quit();
        exit(0);
    });

    //关联主窗口的“发送按钮”，点击按钮后可以将文本编辑框内的内容发送至服务器进行广播
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        QString str = ui->plainTextEdit->toPlainText();
        std::string s = str.toUtf8().data();
        if(send(fd, s.c_str(), s.size(), 0) == -1) {
            perror("send()");
            ::close(this->fd);
            this->close();
            exit(0);
        }
        ui->plainTextEdit->clear();
    });
    display("已进入聊天室");

    //启动接收线程
    recv->start();

}

//子窗口填写用户名和服务器口令
bool MainWindow::SLOP_openNewWidget(){
    login v(this);
    v.exec();
    this->send_message(this->fd,this->list.second);
    if(recv_message(this->fd)=="error"){
        QMessageBox::critical(this,"critical","discorrect password");
        ::close(this->fd);
        return 0;
    }else{
        this->send_message(this->fd,this->list.first);
    }
    return 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

