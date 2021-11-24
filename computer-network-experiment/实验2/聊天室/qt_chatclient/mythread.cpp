#include "mythread.h"
#include "mainwindow.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include<QDebug>
#define maxnum 1024

//线程对象构造函数
recvThread::recvThread(QMainWindow *parent) :QThread(parent){
    pa = parent;
    MainWindow * p=(MainWindow *) pa;
    sockfd = p->fd;
}

//接收线程的运行内容
void recvThread::run(){
    MainWindow * p=(MainWindow *) pa;
    int sockfd = p->fd;
    char buf[maxnum];
    while(true) {
        int num = ::recv(sockfd, buf, maxnum, 0);
        if(num == -1) {
            perror("recv()");
            ::close(sockfd);
            pa->close();
        }
        buf[num]='\0';
        QString str(buf);
        p->display(str);
    }
    return;
}

