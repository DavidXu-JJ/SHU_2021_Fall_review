#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <mythread.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool SLOP_openNewWidget();
    ~MainWindow();
    std::pair<std::string,std::string> list;
    int fd;
    void send_message(const int &sockfd, std::string s);
    std::string recv_message(const int &sockfd);

    friend class recvThread;

private:
    Ui::MainWindow *ui;
public slots:
    void display(QString str);
};
#endif // MAINWINDOW_H
