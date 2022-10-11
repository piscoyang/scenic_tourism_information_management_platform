#include "contact.h"
#include "ui_contact.h"
#include "client.h"
#include <QSqlQuery>
#include <QMessageBox>
contact::contact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::contact)
{
    ui->setupUi(this);
    setWindowTitle("咨询");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

contact::~contact()
{
    delete ui;
}

void contact::on_back_btn_clicked()
{
    this->hide();
    emit this->backToSelect();
}


void contact::on_contact_btn_clicked()
{
    this->hide();
    Client * client=new Client;
    client->getTouristname(this->name);
    client->getTouristID(this->id);
    client->show();
    connect(client,&Client::backToSelect,[=](){
    this->show();
    client->hide();
    });
}


void contact::on_confirm_btn_clicked()
{
    QString select=this->ui->question->currentText();
    QString answer;
    //http://www.szwwco.com/faq/?page=1
    if(select=="为什么不能购买门票？")
    {
        answer="由于疫情防控，景区采用限流措施，请改日再进行买票，祝您生活愉快！";
    }
    else if(select=="门票的三种套餐有何不同？")
    {
        answer="套餐一为150元，套餐二为180元，套餐三为200元。其中，套餐一仅包含门票，套餐二在套餐一基础上包含景区的游览车租凭服务，套餐三在套餐二的基础上包含午餐服务，祝您生活愉快！";
    }
    else if(select=="景区的地址在哪里？")
    {
        answer="广东省深圳市华侨城深南大道9037号，您也可以致电0755-26608000，祝您生活愉快";
    }
    else if(select=="怎样提前了解景区内景点和活动的信息？")
    {
        answer="您可以在登录后的景区信息查询功能中查看，也可以在主页的查询功能中进行查看，祝您生活愉快！";
    }
    else if(select=="小孩可以半价吗？")
    {
        answer="1.2米及一下的小孩无需购买门票，其他均需购买全票，祝您生活愉快！";
    }
    this->ui->answer->setText(answer);
}


void contact::on_confirm_btn2_clicked()
{
    QString message=this->ui->message->toPlainText();
    QSqlQuery query;
    query.exec(QString("insert into message value('%1')").arg(message));
    QMessageBox::information(this,tr("提示"),tr("留言成功"));
    this->ui->message->clear();
}

void contact::getTouristname(QString name)
{
    this->name=name;
}

void contact::getTouristID(QString id)
{
    this->id=id;
}
