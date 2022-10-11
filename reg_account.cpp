#include "reg_account.h"
#include <loginform.h>
#include "ui_reg_account.h"
#include <cstdlib>
#include <ctime>
#include <QSqlQuery>
#include <QCryptographicHash>
Reg_account::Reg_account(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reg_account)
{
    ui->setupUi(this);
    this->setWindowTitle("游客注册");
    connect(ui->exitbtn,&QPushButton::clicked,this,&Reg_account::close);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}


Reg_account::~Reg_account()
{
    //QMessageBox::information(this, "退出", "退出成功！");
    delete ui;
}






void Reg_account::on_backbtn_clicked()
{
    this->hide();
    emit this->backToMain();

}


void Reg_account::on_confirmbtn_clicked()
{
    srand((int)time(nullptr));
    QString name=this->ui->nameinput->text();
    QString sex=this->ui->sexSelect->currentText();
    QString hometown=this->ui->hometowninfo->text();
    QString food=this->ui->foodinfo->text();
    QString person=this->ui->personinfo->text();
    QString psw=this->ui->psw->text();
    QString psw2=this->ui->psw2->text();
    QString psw3=QCryptographicHash::hash(psw.toLocal8Bit(),QCryptographicHash::Md5).toHex();
    if(psw!=psw2||psw.isEmpty())
    {
        QMessageBox::warning(this,"警告","密码不一致或为空",QMessageBox::Yes);
        return;
    }

    QSqlQuery query;
    QString str;
    int num=rand()%10000;
    QString userID="user"+QString::number(num);
    str=QString("insert into tourist values ('%1','%2','%3','%4','%5','%6','%7','')").arg(userID).arg(psw3).arg(name).arg(sex).arg(hometown).arg(food).arg(person);
    query.exec(str);
    QMessageBox::information(this,"注册成功","您的账号为："+userID+"，请返回登录");
    emit this->backToMain();
}

