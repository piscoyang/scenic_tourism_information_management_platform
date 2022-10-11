#include "workermodify.h"
#include "ui_workermodify.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QCryptographicHash>
workermodify::workermodify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::workermodify)
{
    ui->setupUi(this);
    ui->pswinput->setPlaceholderText(tr("添加时输入密码!"));//占位符
    setWindowTitle("修改工作人员基本信息");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

}

workermodify::~workermodify()
{
    delete ui;
}

void workermodify::on_back_clicked()
{
    this->hide();
    emit this->backToManage();
}



void workermodify::on_Modify_clicked()
{
    userId=this->ui->idinput->text();
    name=this->ui->nameinput->text();
    if(name.isEmpty())
    {
        QMessageBox::information(this,"警告","姓名不能为空");
        return;
    }
    sex=this->ui->sexselect->currentText();
    posi=this->ui->posiselect->currentText();
    psw=this->ui->pswinput->text();


    QString str=QString("select * from worker where id=%1").arg(userId);
    QSqlQuery query;
    int record=0;
    query.exec(str);
    query.last();
    record=query.at()+1;
    if(record!=0)
    {
        QString hometowninfo = this->ui->hometowninput->text();
        QString foodinfo = this->ui->foodinput->text();
        QString personinfo = this->ui->personinput->text();
     query.exec(QString("update worker set userName = '%1',"
                        "sex='%2',posi='%3',insuranceinfo1='%4',"
                        "insuranceinfo2='%5',insuranceinfo3='%6' "
                        "where id='%7'").
                arg(name).arg(sex).
                arg(posi).arg(hometowninfo).arg(foodinfo).arg(personinfo).arg(userId));
     QMessageBox::information(this,tr("提示"),tr("修改成功"));
    }
    else
        QMessageBox::information(this,tr("提示"),tr("无此账号"));
}

void workermodify::on_Delete_clicked()
{
    userId=this->ui->idinput->text();
    QString str=QString("select * from worker where id=%1").arg(userId);
    QSqlQuery query;
    int record=0;
    query.exec(str);
    query.last();
    record=query.at()+1;
    if(record!=0)
    {

     query.exec(QString("delete from worker where id='%1'").arg(userId));
     QMessageBox::information(this,tr("提示"),tr("删除成功"));
    }
    else
        QMessageBox::information(this,tr("提示"),tr("无此账号"));

}


void workermodify::on_Insert_clicked()
{
    name=this->ui->nameinput->text();
    if(name.isEmpty())
    {
        QMessageBox::information(this,"警告","姓名不能为空");
        return;
    }
    sex=this->ui->sexselect->currentText();

    posi=this->ui->posiselect->currentText();

    psw=QCryptographicHash::hash(this->ui->pswinput->text().toLocal8Bit(),QCryptographicHash::Md5).toHex();
    if(psw.isEmpty())
        psw = "f379eaf3c831b04de153469d1bec345e";//默认密码666666

    srand((int)time(nullptr));
    int num=2000+rand()%1000;
    userId=QString::number(num);

    QString hometowninfo = this->ui->hometowninput->text();
    if(hometowninfo.isEmpty())hometowninfo = "无";
    QString foodinfo = this->ui->foodinput->text();
    if(foodinfo.isEmpty())foodinfo = "无";
    QString personinfo = this->ui->personinput->text();
    if(personinfo.isEmpty())personinfo = "无";

    QSqlQuery query;
    QString str=QString("insert into worker values ('%1','%2','%3',"
                        "'%4','%5','%6','%7','%8')").
            arg(userId).arg(psw).arg(name).
            arg(sex).arg(posi).
            arg(hometowninfo).
            arg(foodinfo).
            arg(personinfo);
    query.exec(str);
    QMessageBox::information(this,"注册成功","账号为："+userId);


}



void workermodify::on_idinput_textChanged(const QString &arg1)
{
    QString str = QString("select * from worker where id='%1'").arg(arg1);
    QSqlQuery query;
    query.exec(str);
    while(query.next())
    {
        this->ui->nameinput->setText(query.value(2).toString());
        this->ui->sexselect->setCurrentText(query.value(3).toString());
        this->ui->posiselect->setCurrentText(query.value(4).toString());
        this->ui->hometowninput->setText(query.value(5).toString());
        this->ui->foodinput->setText(query.value(6).toString());
        this->ui->personinput->setText(query.value(7).toString());
    }
}

