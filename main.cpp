#include <QApplication>
#include "mainwindow.h"
#include "loginform.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>


int main(int argc,char** argv)
{
    //连接到数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("scenicmanagement");
    db.setUserName("root");
    db.setPassword("");
    bool ok = db.open();
    QApplication app(argc,argv);
    MainWindow *main = new MainWindow();
    main->show();
    if(ok)
    {
        qDebug()<<"数据库连接成功";
        QMessageBox::information(main,"提示","数据库连接成功，欢迎使用本平台!");
    }
    else
        QMessageBox::information(main,"提示","数据库连接失败!");
    app.exec();
    delete main;
    db.close();
    if(!db.isOpen())
    {
        qDebug()<<"数据库已关闭";
    }
    return 0;
}
