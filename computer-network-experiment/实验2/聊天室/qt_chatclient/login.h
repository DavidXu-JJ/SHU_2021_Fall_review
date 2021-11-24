#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(MainWindow *parent = nullptr);
    ~login();

private:
    Ui::login *ui;
    MainWindow *pa;

};

#endif // LOGIN_H
