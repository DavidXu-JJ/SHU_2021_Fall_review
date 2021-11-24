#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <mainwindow.h>
#include <QThread>

namespace Ui {
class recvThread;
}

class recvThread:public QThread
{
public:
    explicit recvThread(QMainWindow *parent = nullptr);
    int sockfd;
    QMainWindow * pa;
    void run() override;
};


#endif // MYTHREAD_H
