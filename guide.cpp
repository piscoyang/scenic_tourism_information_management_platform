#include "guide.h"
#include "ui_guide.h"
#include "leavemessage.h"
#include "touristmanagement.h"
QString Guide::name="";
Guide::Guide(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Guide)
{
    ui->setupUi(this);
    this->setWindowTitle("欢迎回来，导游!");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("ghostwhite"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

Guide::~Guide()
{
    delete ui;
}


void Guide::on_backbtn_clicked()
{
    emit this->backToLogin();
}


void Guide::on_feedbackbtn_clicked()
{
    this->hide();
    Leavemessage * lm=new Leavemessage;
    lm->show();

    connect(lm,&Leavemessage::backToSelect,[=](){
    this->show();
    lm->hide();
    });
}


void Guide::on_managebtn_clicked()
{
    this->hide();
    touristmanagement* tourist=new touristmanagement;
    tourist->getPriority(Guide::name,10);
    tourist->show();
    touristmanagement::operator_name=Guide::name;
    connect(tourist,&touristmanagement::backToLogin,[=](){
    this->show();
    tourist->hide();
    });
}

