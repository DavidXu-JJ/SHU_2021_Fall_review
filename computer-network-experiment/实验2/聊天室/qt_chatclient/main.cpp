#include "mainwindow.h"

#include <QApplication>

//创建主窗口对象，运行其构造函数内容
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
