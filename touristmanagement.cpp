#include "touristmanagement.h"
#include "ui_touristmanagement.h"
#include "touristmodify.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>
QString touristmanagement::operator_name="";
touristmanagement::touristmanagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::touristmanagement)
{
    ui->setupUi(this);
    setWindowTitle("景区游客管理");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

touristmanagement::~touristmanagement()
{
    delete ui;
}



void touristmanagement::on_back_clicked()
{
    this->hide();
    emit this->backToLogin();
}


void touristmanagement::on_search_clicked()
{
    QString str;
    QStringList table_h_headers;
        table_h_headers << "账号" << "姓名" << "性别" ;
    str=QString("select * from tourist");
    if(!touristmanagement::operator_name.isEmpty())str=QString("select * from tourist where guideSelect='%1'").arg(touristmanagement::operator_name);
    QSqlQueryModel *model=new QSqlQueryModel(ui->tableView);
    model->setQuery(str);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("账号"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("姓名"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("性别"));
    model->setHeaderData(7,Qt::Horizontal,QObject::tr("导游选择"));
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(4,true);
    ui->tableView->setColumnHidden(5,true);
    ui->tableView->setColumnHidden(6,true);
     if(!touristmanagement::operator_name.isEmpty())ui->tableView->setColumnHidden(7,true);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(200);
}


void touristmanagement::on_modify_clicked()
{
    this->hide();
    touristmodify* tourist=new touristmodify;
    tourist->show();

    connect(tourist,&::touristmodify::backToManage,[=](){
    this->show();
    tourist->hide();
    });
}

void touristmanagement::getPriority(QString name,int a)
{
    if(a==10)
    {
        this->ui->modify->hide();
    }
}
