#include "workermanagement.h"
#include "ui_workermanagement.h"
#include "workermodify.h"
#include <QSqlQueryModel>

#include <QSqlQuery>
workermanagement::workermanagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::workermanagement)
{
    ui->setupUi(this);
    setWindowTitle("景区人员信息管理");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

workermanagement::~workermanagement()
{
    delete ui;
}



void workermanagement::on_backbtn_clicked()
{
    this->hide();
    emit this->backToLogin();
}


void workermanagement::on_confirmbtn_clicked()
{
    QString select=this->ui->comboBox->currentText();
    QString str;
    QStringList table_h_headers;
        table_h_headers << "账号" << "姓名" << "性别" << "职位";
    if(select=="全部")
    str=QString("select * from worker");
    else if(select=="平台客服")
    str=QString("select * from worker where posi ='平台客服'");
    else
    str=QString("select * from worker where posi ='导游'");
    QSqlQueryModel *model=new QSqlQueryModel(ui->tableView);
    model->setQuery(str);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("账号"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("姓名"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("性别"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("职位"));


    //model->setHeaderData(3,Qt::Horizontal,QObject::tr("图片"));

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(5,true);
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(7,true);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(200);
}


void workermanagement::on_modify_clicked()
{
    this->hide();
    //workermanagement* manage=new workermanagement;
    workermodify* worker=new workermodify;
    worker->show();

    connect(worker,&workermodify::backToManage,[=](){
    this->show();
    worker->hide();
    });
}

