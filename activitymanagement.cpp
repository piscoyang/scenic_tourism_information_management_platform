#include "activitymanagement.h"
#include "ui_activitymanagement.h"
#include "activitydetail.h"
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDate>
#include <QMessageBox>
activitymanagement::activitymanagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::activitymanagement)
{
    ui->setupUi(this);
    setWindowTitle("景区活动管理");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

activitymanagement::~activitymanagement()
{
    delete ui;
}

void activitymanagement::on_pushButton_clicked()
{
    emit this->backToManage();
}


void activitymanagement::on_search_clicked()
{
    QString str;
    QStringList table_h_headers;
        table_h_headers << "活动名称" << "活动地点" << "时间"<<"介绍" ;
    str=QString("select * from activity");
    QSqlQueryModel *model=new QSqlQueryModel(ui->tableView);
    model->setQuery(str);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("活动名称"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("活动地点"));
    //model->setHeaderData(2,Qt::Horizontal,QObject::tr("时间"));
    //model->setHeaderData(3,Qt::Horizontal,QObject::tr("介绍"));
    model->insertColumns(4,1);
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("修改"));
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(2,true);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(200);
    for(int i = 0; i <model->rowCount(); i++)
    {

        QPushButton *button = new QPushButton("确认");
        button->setProperty("activityname",model->data(model->index(i,0)).toString());
        button->setProperty("scenicname",model->data(model->index(i,1)).toString());
        button->setProperty("time",model->data(model->index(i,2)).toString());
        button->setProperty("introduction",model->data(model->index(i,3)).toString());
        connect(button,&QPushButton::clicked,this,&activitymanagement::modify);
        ui->tableView->setIndexWidget(model->index(i, 4), button);
    }
}




void activitymanagement::modify()
{
QPushButton *button=(QPushButton *)sender();
////qDebug()<<button->property("name").toString();
this->hide();
activitydetail *activity=new activitydetail();
activity->getActivityname(button->property("activityname").toString(),button->property("scenicname").toString(),button->property("time").toString(),button->property("introduction").toString());
activity->show();
connect(activity,&activitydetail::backToSelect,[=](){
this->show();
activity->hide();
});
}




void activitymanagement::on_add_btn_clicked()
{
   this->hide();
   activitydetail *activity=new activitydetail();
   activity->show();
   QMessageBox::information(this,"新增","请输入活动名称等信息");
   activity->getActivityname("","","","");
   connect(activity,&activitydetail::backToSelect,[=](){
   this->show();
   activity->hide();
   });
}


