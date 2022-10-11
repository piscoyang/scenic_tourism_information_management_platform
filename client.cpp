#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
     this->setWindowTitle("客户端");
    ui->connectBtn->setStyleSheet("background-color: rgb(6,163,220)");
    ui->sendBtn->setStyleSheet("background-color: rgb(6,163,220)");
    ui->leport->setStyleSheet("color:blue");
    ui->leipAddress->setStyleSheet("color:blue");

    ui->listWidget->setStyleSheet("border:2px solid blue");

    socket = new QTcpSocket(this);
    connectState = false;     //未连接状态

    messageSound = new QSound(":/new/prefix1/sounds/iphone.wav", this);
    connectSound = new QSound(":/new/prefix1/sounds/keke.wav", this);

    this->setWindowIcon(QIcon(":/new/prefix1/image/qq.png"));
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("oldlace"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    connect(socket, &QTcpSocket::readyRead, this, &Client::readMessage);    //接收信息
    connect(socket, &QTcpSocket::disconnected, this, &Client::disconnectSlot);   //打印断开连接信息

}

Client::~Client()
{
    delete ui;
}

void Client::on_connectBtn_clicked()
{
    QString ipStr = ui->leipAddress->text();    //界面显示的地址
    quint16 currentPort = ui->leport->text().toInt();   //界面显示的当前端口
    if(!connectState)    //客户端还未连接服务端
    {
        socket->connectToHost(ipStr, currentPort);   //连接服务端
        if(socket->waitForConnected())   //等待连接成功
        {
            ui->listWidget->addItem("连接成功");
            ui->connectBtn->setText("关闭连接");
            connectSound->play();
            connectState = true;
        }

        else     //连接失败
        {
            QMessageBox::warning(this, "连接失败", socket->errorString());   //连接错误信息提醒
        }
    }

    else   //客户端已经连接
    {
        socket->close();   //关闭套接字，此时会发送disconnected信号
        connectSound->play();
        ui->connectBtn->setText("连接");
    }
}


void Client::on_sendBtn_clicked()
{
    QString str = ui->textSend->toPlainText();
    if(socket->isOpen() && socket->isValid())
    {
        str = "游客："+this->id+'\n' + str;
        socket->write(str.toUtf8());    //给服务端发送信息
        ui->textSend->clear();
    }

    QString showStr = QDateTime::currentDateTime().toString("dddd yyyy.MM.dd hh:mm:ss") + '\n'+ str;
    ui->textReceive->append(showStr);     //显示自己发送的信息
}
void Client::readMessage()    //接收信息
{
    messageSound->play();
    QByteArray arr = socket->readAll();
    QString str;
    str = QDateTime::currentDateTime().toString("dddd yyyy.MM.dd hh:mm:ss") + '\n' +"平台客服\n"+ arr.data();
    ui->textReceive->append(str);     //显示信息
}


void Client::disconnectSlot()    //打印连接断开信息
{
    ui->listWidget->addItem("clint disconnected");
}

void Client::on_back_btn_clicked()
{
    this->hide();
    this->backToSelect();
}

void Client::getTouristname(QString name)
{
    this->name=name;
}

void Client::getTouristID(QString id)
{
    this->id=id;
}
