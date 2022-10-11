#include "ticketreturn.h"
#include "ui_ticketreturn.h"
#include "contact.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
Ticketreturn::Ticketreturn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ticketreturn)
{
    ui->setupUi(this);
    this->setWindowTitle("退票");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

Ticketreturn::~Ticketreturn()
{
    delete ui;
}
void Ticketreturn::getTouristname(QString name)
{
    this->name=name;
    QString str1=QString("select * from `order` where customer_name='%1'").arg(name);
    QSqlQuery query;
    query.exec(str1);
    if(!query.next())
    {
        QMessageBox::information(this,tr("提示"),tr("您无订单!"));
        this->hide();
        emit this->backToSelect();
        return;
    }
    else
    {
        query.exec(str1);
        while(query.next())
        {
        this->ui->order_number->setText(query.value(3).toString());
        this->ui->package_2->setText(query.value(0).toString());
        this->ui->price->setText(query.value(4).toString());
        this->ui->guide->setText(query.value(5).toString());
        this->ui->time->setDateTime(query.value(2).toDateTime());
        }
    }
}
void Ticketreturn::on_back_btn_clicked()
{
    this->hide();
    emit this->backToSelect();
}


void Ticketreturn::on_consult_btn_clicked()
{
    this->hide();
    contact* con=new contact;
    con->show();
    connect(con,&contact::backToSelect,[=](){
        this->show();
        con->hide();
    });
}


void Ticketreturn::on_pushButton_clicked()
{
    QString isReturm=this->ui->returnSelect->currentText();
    if(isReturm=="是")
    {
        QString customer_name=this->name;
        QSqlQuery query;
         query.exec(QString("delete from `order` where customer_name='%1'").arg(customer_name));
         QMessageBox::information(this,tr("提示"),tr("退票成功!"));
         this->hide();
         emit this->backToSelect();

    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("取消退票!"));
        this->hide();
        emit this->backToSelect();
    }
}

