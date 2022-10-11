#include "admin.h"
#include "ui_admin.h"
#include "loginform.h"
#include "workermanagement.h"
#include "touristmanagement.h"
#include "scenicmanagement.h"
#include "activitymanagement.h"
#include "order_information.h"
#include <QDebug>
Admin::Admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Admin)
{

    ui->setupUi(this);
    this->setWindowTitle("欢迎，Admin!");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("greenyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

Admin::~Admin()
{
    delete ui;
}

void Admin::on_exitbtn_clicked()
{
    emit this->backToLogin();
}


void Admin::on_workermanage_clicked()
{
    this->hide();
    workermanagement* manage=new workermanagement;
    manage->show();

    connect(manage,&workermanagement::backToLogin,[=]() ->void {
    this->show();
    manage->hide();
    });
}


void Admin::on_touristmanage_clicked()
{
    this->hide();
    touristmanagement* tourist=new touristmanagement;
    tourist->show();

    connect(tourist,&touristmanagement::backToLogin,[=](){
    this->show();
    tourist->hide();
    });
}


void Admin::on_scenicmanage_clicked()
{
  this->hide();
  Scenicmanagement* scenic=new Scenicmanagement;
  scenic->show();
  connect(scenic,&Scenicmanagement::backToManage,[=](){
   this->show();
   scenic->hide();
   });
}


void Admin::on_activitymanage_clicked()
{
    this->hide();
    activitymanagement* activity=new activitymanagement;
    activity->show();
    connect(activity,&activitymanagement::backToManage,[=](){
     this->show();
     activity->hide();
     });
}


void Admin::on_ordermanagement_btn_clicked()
{
    this->hide();
    Order_information* order=new Order_information;
    order->show();
    connect(order,&Order_information::backToSelect,[=](){
     this->show();
     order->hide();
     });
}

