#include "leavemessage.h"
#include "ui_leavemessage.h"
#include "QSqlQueryModel"

Leavemessage::Leavemessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Leavemessage)
{
    ui->setupUi(this);
    this->setWindowTitle("游客留言信息");
    QStringList table_h_headers;
    table_h_headers << "留言信息" ;
    QString str="select * from message";
    QSqlQueryModel *model=new QSqlQueryModel(ui->tableView);
    model->setQuery(str);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("留言信息"));
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(200);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

Leavemessage::~Leavemessage()
{
    delete ui;
}

void Leavemessage::on_pushButton_clicked()
{
    this->hide();
    emit this->backToSelect();
}

