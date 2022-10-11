#include "ticketbuy.h"
#include "ui_ticketbuy.h"
#include "contact.h"
#include "order_information.h"
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>

TicketBuy::TicketBuy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TicketBuy)
{
    ui->setupUi(this);
    this->setWindowTitle("购票");
    this->guideSelectloading();
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

TicketBuy::~TicketBuy()
{
    delete ui;
}
void TicketBuy::guideSelectloading()
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
void TicketBuy::on_back_btn_clicked()
{
    this->hide();
    emit this->backToSelect();
}

void TicketBuy::getTouristname(QString name)
{
    this->name=name;
}

void TicketBuy::on_consult_btn_clicked()
{
    this->hide();
    contact* con=new contact;
    con->show();
    connect(con,&contact::backToSelect,[=](){
        this->show();
        con->hide();
    });
}

bool TicketBuy::isQualified()
{

    QString str="select tourist_number from admin";
    QSqlQuery query;
    query.exec(str);
    int number1,number2;
    while(query.next())number1=query.value(0).toInt();

    QDate current_date = QDate::currentDate();
    QString currentDate = current_date.toString("yyyy-MM-dd");

    str=QString("select count(*) from `order` where time like '%%1%'").arg(currentDate);
    query.exec(str);
    while(query.next())number2=query.value(0).toInt();

    if(number2==number1)return false;

    else
        return true;
}
void TicketBuy::on_pushButton_clicked()
{
    bool hasQualified=isQualified();
    if(hasQualified==false)
    {
        QMessageBox::information(this,tr("提示"),tr("当日景区已经达到最大容量，请改日买票!"));
       this->hide();
       emit this->backToSelect();
        return;
    }
    QString str1=QString("select * from `order` where customer_name='%1'").arg(this->name);
    QSqlQuery isExist;
    isExist.exec(str1);
    if(isExist.next())
    {
        QMessageBox::information(this,tr("提示"),tr("已经购买!"));
       this->hide();
        emit this->backToSelect();
        return;
    }

    QString package=this->ui->packageSelect->currentText();
    QString guide=this->ui->guideSelect->currentText();
    double price;
    if(package=="套餐一 150元")
    {
        price=150;
        package="套餐一";
    }
    else if(package=="套餐一 180元")
    {
        price=180;
        package="套餐二";
    }
    else
    {
        price=200;
        package="套餐三";
    }
    QString touristname=this->name;
    QDateTime time = QDateTime::currentDateTime(); //获取当前时间
    int cut_timestamp = time.toTime_t(); //将当前时间转为时间戳
    QString order_number=QString::number(cut_timestamp);
    //qDebug()<<order_number<<" "<<touristname<<" "<<time.toString("yyyy-MM-dd hh:mm:ss")<<" "<<package<<" "<<price<<" "<<guide;
    QSqlQuery query;
    //INSERT into `order` VALUES("1649514369","杨澜","2022-04-09 22:26:09","套餐一",150,"李五");
    QString str2=QString("insert into `order` values('%1','%2','%3','%4',%5,'%6')").arg(order_number).arg(touristname).arg(time.toString("yyyy-MM-dd hh:mm:ss")).arg(package).arg(price).arg(guide);
    query.exec(str2);
    str2=QString("update tourist set guideSelect='%1' where userName='%2'").arg(guide).arg(touristname);
    query.exec(str2);
     QMessageBox::information(this,tr("提示"),tr("购买成功！"));
    this->hide();
     emit this->backToSelect();

}

