#include "activitydetail.h"
#include "ui_activitydetail.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
void activitydetail::scenicnameloading()
{
    QSqlQuery query;
    QString str="select name from scenic";
    query.exec(str);
    while(query.next())
    {
        QString scenicSelect=query.value(0).toString();
        this->ui->scenicname->addItem(scenicSelect);
    }
}
activitydetail::activitydetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::activitydetail)
{
    ui->setupUi(this);
    scenicnameloading();
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

activitydetail::~activitydetail()
{
    delete ui;
}

void activitydetail::on_back_btn_clicked()
{
    this->hide();
    this->backToSelect();
}

void activitydetail::getActivityname(QString activityname,QString scenicname,QString gettime,QString introduction)
{
    setWindowTitle(activityname);
    if(activityname=="")this->ui->activity_name->setEnabled(true);
    if(activityname=="detail")
    {
        this->ui->activity_name->setEnabled(false);
        this->ui->scenicname->setEnabled(false);
        this->ui->introduction->setEnabled(false);
    }
    this->ui->activity_name->setText(activityname);
    this->ui->scenicname->setCurrentText(scenicname);
    gettime=gettime.left(8);
    this->ui->time->setText(gettime);
    this->ui->introduction->setText(introduction);
    QString photo_path = QString(":/scenic/images/%1.png").arg(activityname);
    QString imgPath = QString(photo_path);
    ui->activitypicture->setPixmap(QPixmap(imgPath));//label 加载图片imgpath
    ui->activitypicture->setScaledContents(true);    //根据label大小缩放图片

}
void activitydetail::on_modify_btn_clicked()
{
    QString activityname=this->ui->activity_name->text();
    QString scenicname=this->ui->scenicname->currentText();
    QString time=this->ui->time->text();

    QString introduction=this->ui->introduction->toPlainText();
    QSqlQuery query;
    query.exec(QString("update activity set scenicname='%1',time='%2',introduction='%3' where activityname='%4'").arg(scenicname).arg(time).arg(introduction).arg(activityname));
    QMessageBox::information(this,tr("提示"),tr("修改成功"));
    this->hide();
    emit this->backToSelect();
}


void activitydetail::on_add_btn_clicked()
{

    QSqlQuery query;
    QString activityname=this->ui->activity_name->text();
    QString scenicname=this->ui->scenicname->currentText();
    QString time=this->ui->time->text();
    QString introduction=this->ui->introduction->toPlainText();

    if(activityname!="")
    {
        QString str=QString("insert into activity values ('%1','%2','%3','%4')").arg(activityname).arg(scenicname).arg(time).arg(introduction);
        query.exec(str);
        QMessageBox::information(this,"成功","增加活动成功");
        this->hide();
        emit this->backToSelect();
    }
    else
    {
        QMessageBox::information(this,"失败","活动名不能为空");
    }

}


void activitydetail::on_delete_btn_clicked()
{
    QSqlQuery query;
    query.exec(QString("delete from activity where activityname='%1'").arg(this->ui->activity_name->text()));
    QMessageBox::information(this,tr("提示"),tr("删除成功"));
    this->hide();
    emit this->backToSelect();
}
  void activitydetail::Lock(QString message)
  {
      if(!message.isEmpty())
      {
          this->ui->modify_btn->hide();
          this->ui->add_btn->hide();
          this->ui->delete_btn->hide();
          this->ui->scenicname->setEnabled(false);
          this->ui->time->setEnabled(false);
      }
  }

