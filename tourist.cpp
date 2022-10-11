#include "tourist.h"
#include "ui_tourist.h"
#include "loginform.h"
#include "ticketbuy.h"
#include "ticketreturn.h"
#include "recommend.h"
#include "search.h"
#include "contact.h"

Tourist::Tourist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tourist)
{
    ui->setupUi(this);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("ghostwhite"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

Tourist::~Tourist()
{
    delete ui;
}


void Tourist::getTouristname(QString name)
{
    this->name=name;
}

void Tourist::getTouristId(QString id)
{
    this->id=id;
}

void Tourist::on_ticketbuy_btn_clicked()
{
    this->hide();
    TicketBuy* ticket=new TicketBuy;
    ticket->getTouristname(this->name);
    ticket->show();
    connect(ticket,&TicketBuy::backToSelect,[=](){
        this->show();
        ticket->hide();
    });
}


void Tourist::on_ticketreturn_btn_clicked()
{
    this->hide();
    Ticketreturn* ticket=new Ticketreturn;
    ticket->getTouristname(this->name);
    ticket->show();

    connect(ticket,&Ticketreturn::backToSelect,[=](){
        this->show();
        ticket->hide();
    });
}


void Tourist::on_consult_btn_clicked()
{
    this->hide();
    contact* con=new contact;
    con->getTouristname(this->name);
    con->getTouristID(this->id);
    con->show();
    connect(con,&contact::backToSelect,[=](){
        this->show();
        con->hide();
    });
}


void Tourist::on_recommend_btn_clicked()
{
    this->hide();
    Recommend* secnic=new Recommend;
    secnic->show();
    connect(secnic,&Recommend::backToMain,[=](){
        this->show();
        secnic->hide();
    });
}



void Tourist::on_search_btn_clicked()
{
    this->hide();
    Search* sea=new Search;
    sea->show();
    connect(sea,&Search::backToMain,[=](){
        this->show();
        sea->hide();
    });
}


void Tourist::on_back_btn_clicked()
{
        this->hide();
        emit this->backToLogin();
}

