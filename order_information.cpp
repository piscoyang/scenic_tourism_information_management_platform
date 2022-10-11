#include "order_information.h"
#include "ui_order_information.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QMessageBox>

Order_information::Order_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Order_information)
{
    ui->setupUi(this);
    this->setWindowTitle("订单/总收入查看");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

}

Order_information::~Order_information()
{
    delete ui;
}


void Order_information::on_back_btn_clicked()
{
    this->hide();
    this->backToSelect();
}

void Order_information::orderloading()
{
    QStringList table_h_headers;
           QString str="select * from `order`";
           table_h_headers << "订单号" << "游客姓名" << "时间"<<"套餐"<<"价格"<<"导游" ;
           QSqlQueryModel *model=new QSqlQueryModel(this->ui->order);
           model->setQuery(str);
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("订单号"));
           model->setHeaderData(1,Qt::Horizontal,QObject::tr("游客姓名"));
           model->setHeaderData(2,Qt::Horizontal,QObject::tr("时间"));
           model->setHeaderData(3,Qt::Horizontal,QObject::tr("套餐"));
           model->setHeaderData(4,Qt::Horizontal,QObject::tr("价格"));
           model->setHeaderData(5,Qt::Horizontal,QObject::tr("导游"));
           this->ui->order->setModel(model);
           this->ui->order->horizontalHeader()->setMinimumSectionSize(200);
}



void Order_information::on_confirm_clicked()
{
    QSqlQueryModel *model=new QSqlQueryModel(this->ui->order);
    model->clear();
    QStringList table_h_headers;
    QString str="select * from `order`";
    QString select=this->ui->searchSelect->currentText();
    QString input=this->ui->input->text();
    if(select=="默认")str+="";
    //select * from `order` where order_number like '%171%%'
    else if(select=="订单号")str+=QString(" where order_number like '%%1%'").arg(input);
    else if(select=="游客姓名")str+=QString(" where customer_name like '%%1%'").arg(input);
    else if(select=="时间")str+=QString(" where time like '%%1%'").arg(input);
    else if(select=="套餐")str+=QString(" where package_selection like '%%1%'").arg(input);
    else
        str+=QString(" where guide_name like '%%1%'").arg(input);

    table_h_headers << "订单号" << "游客姓名" << "时间"<<"套餐"<<"价格"<<"导游" ;
    model->setQuery(str);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("订单号"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("游客姓名"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("时间"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("套餐"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("价格"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("导游"));
    QSqlQuery query;
    QString string="SELECT SUM(price) from  `order`";
    query.exec(string);
    while(query.next())this->ui->income->setText(query.value(0).toString()+"元");
    this->ui->order->setModel(model);
    this->ui->order->horizontalHeader()->setMinimumSectionSize(200);
}




void Order_information::on_confirm_2_clicked()
{

    int number=(this->ui->lineEdit->text()).toInt();
    QString str=QString("update admin set tourist_number=%1").arg(number);
    QSqlQuery query;
    query.exec(str);
    QMessageBox::information(this,tr("提示"),tr("修改游客数量成功！"));
}

void Order_information::getPriority(int a)
{
    if(a==10)
    {
        this->ui->income->hide();
        this->ui->label_4->hide();
        this->ui->lineEdit->hide();
        this->ui->confirm_2->hide();
        this->ui->label_3->hide();
    }
}
