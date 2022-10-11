#include "manager.h"
#include "ui_manager.h"
#include "touristmodify.h"
#include "server.h"
#include "order_information.h"
Manager::Manager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Manager)
{
    ui->setupUi(this);
    this->setWindowTitle("欢迎回来，平台客服!");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("ghostwhite"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

Manager::~Manager()
{
    delete ui;
}

void Manager::on_pushButton_clicked()
{
    emit this->backToLogin();
}


void Manager::on_tourist_modify_btn_clicked()
{
    this->hide();
    touristmodify* tourist=new touristmodify;
    tourist->show();

    connect(tourist,&::touristmodify::backToManage,[=](){
    this->show();
    tourist->hide();
    });
}


void Manager::on_contact_btn_clicked()
{
    this->hide();
    Server* server=new Server;
    server->show();

    connect(server,&Server::backToSelect,[=](){
    this->show();
    server->hide();
    });
}


void Manager::on_oder_lookup_clicked()
{
    this->hide();
    Order_information* order=new Order_information;
    order->show();
    order->getPriority(10);
    connect(order,&Order_information::backToSelect,[=](){
     this->show();
     order->hide();
     });
}

