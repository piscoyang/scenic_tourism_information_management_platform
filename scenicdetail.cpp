#include "scenicdetail.h"
#include "ui_scenicdetail.h"
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
scenicdetail::scenicdetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scenicdetail)
{
    ui->setupUi(this);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

}



scenicdetail::~scenicdetail()
{
    delete ui;
}

void scenicdetail::on_pushButton_clicked()
{
    this->hide();
    emit this->backToSelect();
}

void scenicdetail::getData(int id,QString name,QString area)
{
    this->id=id;
    setWindowTitle(name);
    ui->name->setText(name);
    ui->area->setText(area);
    ui->modify_btn->hide();
    QString photo_path = QString(":/scenic/images/%1.jpg").arg(id);
    QString imgPath = QString(photo_path);
    ui->scenicpicture->setPixmap(QPixmap(imgPath));//label 加载图片imgpath
    ui->scenicpicture->setScaledContents(true);    //根据label大小缩放图片
    QSqlQuery query;
    QString str=QString("select introduction from scenic where name='%1'").arg(name);
    query.exec(str);
    QString introduction;
    while(query.next())
    {
        introduction=query.value(0).toString();
    }
    ui->scenicintroduction->setText(introduction);

}


void scenicdetail::getScenicData(QString scenicid, QString scenicname, QString scenicarea, QString scenicinfo)
{
    if(!scenicid.isEmpty())
    {
        this->id=scenicid;
        this->ui->name->setEnabled(true);
        this->ui->area->setEnabled(true);
        this->ui->scenicintroduction->setEnabled(true);
    }
    setWindowTitle(scenicname);
    ui->name->setText(scenicname);
    ui->area->setText(scenicarea);
    ui->scenicintroduction->setText(scenicinfo);
    QString photo_path = QString(":/scenic/images/%1.jpg").arg(scenicid);
    QString imgPath = QString(photo_path);
    ui->scenicpicture->setPixmap(QPixmap(imgPath));//label 加载图片imgpath
    ui->scenicpicture->setScaledContents(true);    //根据label大小缩放图片

}

void scenicdetail::on_modify_btn_clicked()
{
    QString scenicname=this->ui->name->text();
    QString scenicarea=this->ui->area->text();
    QString introduction=this->ui->scenicintroduction->toPlainText();
    QSqlQuery query;
    query.exec(QString("update scenic set name='%1',area='%2',introduction='%3' where id=%4").
               arg(scenicname).arg(scenicarea).arg(introduction).arg(this->id));
    QMessageBox::information(this,tr("提示"),tr("修改成功"));
    this->hide();
    emit this->backToSelect();
}

