#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <loginform.h>
#include <reg_account.h>
#include <search.h>
#include <recommend.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug()<<"欢迎使用本平台";
    ui->setupUi(this);
    this->setWindowTitle("景区旅游信息管理平台");
}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug()<<"欢迎下次使用本平台";
}

void MainWindow::on_login_btn_clicked()
{
    this->hide();
    LoginForm *login=new LoginForm;
    login->show();
    connect(login,&LoginForm::backToMain,[=](){
        this->show();
        login->hide();
    });
}


void MainWindow::on_register_btn_clicked()
{
    this->hide();
    Reg_account *reg=new Reg_account;
    reg->show();
    connect(reg,&Reg_account::backToMain,[=](){
        this->show();
        reg->hide();
    });
}


void MainWindow::on_search_btn_clicked()
{
    this->hide();
    Search* sea=new Search;
    sea->show();
    connect(sea,&Search::backToMain,[=](){
        this->show();
        sea->hide();
    });
}


void MainWindow::on_recommend_btn_clicked()
{
    this->hide();
    Recommend* secnic=new Recommend;
    secnic->show();
    connect(secnic,&Recommend::backToMain,[=](){
        this->show();
        secnic->hide();
    });
}


void MainWindow::on_exit_btn_clicked()
{
    emit close();
}

