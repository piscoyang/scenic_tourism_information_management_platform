#include "touristmodify.h"
#include "ui_touristmodify.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
touristmodify::touristmodify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::touristmodify)
{
    ui->setupUi(this);
    guideSelectloading();
    setWindowTitle("修改游客基本信息");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}
void touristmodify::guideSelectloading()
{
    QSqlQuery query;
    QString str="select username from worker where posi='导游'";
    query.exec(str);
    while(query.next())
    {
        QString guideSelect=query.value(0).toString();
        this->ui->guideSelect->addItem(guideSelect);
    }
}
touristmodify::~touristmodify()
{
    delete ui;
}

void touristmodify::on_Modify_clicked()
{
    QString userId=this->ui->idinput->text();
    QString name=this->ui->nameinput->text();
    QString sex=this->ui->sexselect->currentText();
    QString guideSelect=this->ui->guideSelect->currentText();
    //qDebug()<<userId<<" "<<name<<" "<<sex;
    QString str=QString("select * from tourist where id='%1'").arg(userId);
    QSqlQuery query;
    int record=0;
    query.exec(str);
    query.last();
    record=query.at()+1;
    QString update=QString("update tourist set userName = '%1',sex='%2',guideSelect='%3' where id='%4'").arg(name).arg(sex).arg(guideSelect).arg(userId);
    if(name=="")update=QString("update tourist set sex='%1',guideSelect='%2' where id='%3'").arg(sex).arg(guideSelect).arg(userId);

    if(record!=0)
    {
     query.exec(update);
     QMessageBox::information(this,tr("提示"),tr("修改成功"));
    }
    else
        QMessageBox::information(this,tr("提示"),tr("无此账号"));
}


void touristmodify::on_Delete_clicked()
{
    QString userId=this->ui->idinput->text();
    QString str=QString("select * from tourist where id='%1'").arg(userId);
    QSqlQuery query;
    int record=0;
    query.exec(str);
    query.last();
    record=query.at()+1;
    if(record!=0)
    {

     query.exec(QString("delete from tourist where id='%1'").arg(userId));
     QMessageBox::information(this,tr("提示"),tr("删除成功"));
    }
    else
        QMessageBox::information(this,tr("提示"),tr("无此账号"));
}


void touristmodify::on_back_clicked()
{
    this->hide();
    emit this->backToManage();
}


void touristmodify::on_idinput_textChanged(const QString &arg1)
{
    QString str = QString("select * from tourist where id='%1'").arg(arg1);
    QSqlQuery query;
    query.exec(str);
    while(query.next())
    {
        this->ui->nameinput->setText(query.value(2).toString());
        this->ui->sexselect->setCurrentText(query.value(3).toString());
        this->ui->guideSelect->setCurrentText(query.value(7).toString());
    }
}

