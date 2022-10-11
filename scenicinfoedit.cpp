#include "scenicinfoedit.h"
#include "ui_scenicinfoedit.h"
#include "scenicdetail.h"
#include <QSqlQueryModel>
ScenicInfoEdit::ScenicInfoEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScenicInfoEdit)
{
    ui->setupUi(this);
    setWindowTitle("景区活动管理");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

ScenicInfoEdit::~ScenicInfoEdit()
{
    delete ui;
}

void ScenicInfoEdit::on_pushButton_clicked()
{
    emit this->backToPrevious();
    this->hide();
}


void ScenicInfoEdit::on_search_clicked()
{
    QString str;
        QStringList table_h_headers;
            table_h_headers << "景点序号" << "景点名称" << "景点区域"<<"介绍" ;
        str=QString("select * from scenic");
        QSqlQueryModel *model=new QSqlQueryModel(ui->tableView);
        model->setQuery(str);
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("景点序号"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("景点名称"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("景点区域"));
        model->setHeaderData(3,Qt::Horizontal,QObject::tr("景点介绍"));
        model->insertColumns(4,1);
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("修改"));
        ui->tableView->setModel(model);
        ui->tableView->horizontalHeader()->setMinimumSectionSize(200);
        for(int i = 0; i <model->rowCount(); i++)
        {

            QPushButton *button = new QPushButton("确认");
            button->setProperty("scenicid",model->data(model->index(i,0)).toString());
            button->setProperty("scenicname",model->data(model->index(i,1)).toString());
            button->setProperty("scenicarea",model->data(model->index(i,2)).toString());
            button->setProperty("scenicinfo",model->data(model->index(i,3)).toString());
            connect(button,&QPushButton::clicked,this,&ScenicInfoEdit::modify);
            ui->tableView->setIndexWidget(model->index(i, 4), button);
        }
}

void ScenicInfoEdit::modify()
{
QPushButton *button=(QPushButton *)sender();
////qDebug()<<button->property("name").toString();
this->hide();
scenicdetail *scenic=new scenicdetail();
scenic->getScenicData(button->property("scenicid").toString(),button->property("scenicname").toString(),button->property("scenicarea").toString(),button->property("scenicinfo").toString());
scenic->show();
connect(scenic,&scenicdetail::backToSelect,[=](){
this->show();
scenic->hide();
});
}

