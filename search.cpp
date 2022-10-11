#include "search.h"
#include "ui_search.h"
#include "loginform.h"
#include <QTableWidget>
#include <QSqlQueryModel>
#include <QPushButton>
#include <QSqlQuery>
#include <QDebug>
#include <scenicdetail.h>
#include "activitydetail.h"
Search::Search(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
    this->setWindowTitle("查询");
     connect(this->ui->backbtn,&QPushButton::clicked,this,&Search::on_backbtn_clicked);
     QPalette palette(this->palette());
     palette.setColor(QPalette::Background, QColor("lightyellow"));
     this->setAutoFillBackground(true);
     this->setPalette(palette);

}

Search::~Search()
{
    delete ui;
}

void Search::on_pushButton_clicked()
{
    QString select=this->ui->searchselect->currentText();
    QString input=this->ui->input->text();
    QString str;
    QStringList table_h_headers;
        table_h_headers << "编号"<<"名称" << "区域" << "介绍"<<"详情";
    if(select=="按区域查询")
        str=QString("select * from scenic where area like '%%1%%'").arg(input);
    else if(select=="按名称查询")
    str=QString("select * from scenic where name like '%%1%%'").arg(input);
    else
        str=QString("select * from activity where activityname like '%%1%%'").arg(input);
    QSqlQueryModel *model=new QSqlQueryModel(ui->tableView);
    if(select=="按区域查询"||select=="按名称查询")
    {
        model->setQuery(str);
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("景点编号"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("景点名称"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("所属区域"));
        model->setHeaderData(3,Qt::Horizontal,QObject::tr("介绍"));
        model->insertColumns(4,1);
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("点击详情"));

        ui->tableView->setModel(model);
        //qDebug()<<ui->tableView->model()->rowCount();

        for(int i = 0; i <model->rowCount(); i++)
        {

            QPushButton *button = new QPushButton("确认");
            button->setProperty("id",model->data(model->index(i,0)).toString());
            button->setProperty("name",model->data(model->index(i,1)).toString());
            button->setProperty("area",model->data(model->index(i,2)).toString());
            //qDebug()<<model->data(model->index(i,0)).toString();
            connect(button,&QPushButton::clicked,this,&Search::introbtn_clicked);

            ui->tableView->setIndexWidget(model->index(i, 4), button);
        }
    }
    else
    {
        QStringList table_h_headers;
        table_h_headers << "活动名称" << "活动地点" << "时间"<< "介绍" ;
        QSqlQueryModel *model=new QSqlQueryModel(ui->tableView);
        model->setQuery(str);
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("活动名称"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("活动地点"));
        //model->setHeaderData(2,Qt::Horizontal,QObject::tr("时间"));
        //model->setHeaderData(3,Qt::Horizontal,QObject::tr("介绍"));
        model->insertColumns(4,1);
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("查看详细信息"));
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
            connect(button,&QPushButton::clicked,this,&Search::detail);
            ui->tableView->setIndexWidget(model->index(i, 4), button);
        }
    }


}


void Search::on_backbtn_clicked()
{
    this->hide();
    emit this->backToMain();
}

void Search::introbtn_clicked()
{
    QPushButton *button=(QPushButton *)sender();
    //qDebug()<<button->property("name").toString();
    this->hide();
    scenicdetail *scenic=new scenicdetail();
    scenic->getData(button->property("id").toInt(),button->property("name").toString(),button->property("area").toString());
    scenic->show();
    connect(scenic,&scenicdetail::backToSelect,[=](){
    this->show();
    scenic->hide();
    });
}
void Search::detail()
{
    QPushButton *button=(QPushButton *)sender();
    this->hide();
    activitydetail *activity=new activitydetail();
    activity->show();
    activity->getActivityname(button->property("activityname").toString(),button->property("scenicname").toString(),button->property("time").toString(),button->property("introduction").toString());
    activity->Lock("sss");
    connect(activity,&activitydetail::backToSelect,[=](){
    this->show();
    activity->hide();
    });
}
