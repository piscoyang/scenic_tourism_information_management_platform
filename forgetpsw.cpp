#include "forgetpsw.h"
#include "loginform.h"
#include "ui_forgetpsw.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QCryptographicHash>
Forgetpsw::Forgetpsw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Forgetpsw)
{
    ui->setupUi(this);
    this->setWindowTitle("忘记密码");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

Forgetpsw::~Forgetpsw()
{
    delete ui;
}

void Forgetpsw::on_backbtn_clicked()
{
    emit this->backToLogin();
}


void Forgetpsw::on_confirm_clicked()
{

    QString userId=this->ui->idinput->text();
    QString hometownInfo=this->ui->howntowninfo->text();
    QString foodInfo=this->ui->foodinfo->text();
    QString personInfo=this->ui->personinfo->text();
    QString userType=this->ui->objectselect->currentText();
    if(userType=="工作人员")userType="worker";
    else
        userType="tourist";
    QSqlQuery query;
    QString str=QString("select * from %1 where id='%2' and insuranceinfo1='%3' and insuranceinfo2='%4' and insuranceinfo3='%5'")
            .arg(userType).arg(userId).arg(hometownInfo).arg(foodInfo).arg(personInfo);
    //QString str="select * from tourist where id=1001";
    //select * from tourist where  id=1001 and insuranceinfo1="北京" and insuranceinfo2="可乐" and insuranceinfo3="母亲";
    //UPDATE `worker` SET `insuranceinfo1`='上海', `insuranceinfo2`='北京烤鸭', `insuranceinfo3`='兄弟' WHERE (`id`='2001')
    int record=0;
    query.exec(str);
    query.last();
    record=query.at()+1;
    if(record!=0)
    {
        //QMessageBox::warning(this, tr("Welcome！"),tr("欢迎登录！"),QMessageBox::Yes);
        QString pswrecv=this->ui->psw->text();
        QString pswrecv2=this->ui->psw2->text();
        QString psw=QCryptographicHash::hash(pswrecv.toLocal8Bit(),QCryptographicHash::Md5).toHex();
        if(pswrecv==""){
            QMessageBox::warning(this, tr("警告！"),tr("密码不能为空！"),QMessageBox::Yes);
        }
        if(pswrecv!=pswrecv2||pswrecv.isEmpty())
        {

            QMessageBox::warning(this, tr("警告！"),tr("两次输入的密码不一致，请重新输入"),QMessageBox::Yes);
            this->ui->psw->clear();
            this->ui->psw2->clear();
            this->ui->psw->setFocus();
        }
        else
        {
            query.exec(QString("update %1 set psw = '%2' where id='%3'").arg(userType).arg(psw).arg(userId));
            QMessageBox::information(this,tr("提示"),tr("修改成功,请返回登录"));
            emit this->backToLogin();
        }
    }
    else
    {
        QMessageBox::warning(this, tr("警告！"),tr("账号或密保信息输入错误！"),QMessageBox::Yes);

    }

}

