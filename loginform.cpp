#include "loginform.h"
#include "reg_account.h"
#include "forgetpsw.h"
#include "admin.h"
#include "tourist.h"
#include "manager.h"
#include "guide.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QDebug>
LoginForm::LoginForm(QDialog *parent) :
    QDialog(parent)
{
    //设置窗体标题
    this->setWindowTitle(tr("登录界面"));

    this->resize(300,250);
    //登录方式
    logintype = new QLabel(this);
    logintype->move(50,55);
    logintype->setText("登录方式:");


    type=new QComboBox(this);
    type->move(120,55);
    type->addItem("管理员");
    type->addItem("工作人员");
    type->addItem("游客");
    //用户名Label
    userNameLbl = new QLabel(this);   //new一个标签对象
    userNameLbl->move(80,80);         //移动到(70,80)位置(Label左上角坐标，相对于父窗体)
    userNameLbl->setText("账号:");  //设置标签文本



    //用户名输入框
    userNameLEd = new QLineEdit(this);
    userNameLEd->move(120,80);
    userNameLEd->setPlaceholderText(tr("请输入账号!"));//占位符

    //密码Label
    pwdLbl = new QLabel(this);
    pwdLbl->move(80,130);
    pwdLbl->setText("密码:");

    //密码输入框
    pwdLEd = new QLineEdit(this);
    pwdLEd->move(120,130);
    pwdLEd->setPlaceholderText("请输入密码!");
    pwdLEd->setEchoMode(QLineEdit::Password);//输入的密码以圆点显示

    //登录按钮
    loginBtn = new QPushButton(this);
    loginBtn->move(80,170);
    loginBtn->setText("登录");

    //忘记密码按钮
    forgetpsw =new QPushButton(this);
    forgetpsw->move(80,200);
    forgetpsw->setText("忘记密码");

    //注册
    reg =new QPushButton(this);
    reg->move(170,170);
    reg->setText("游客注册");

    //退出按钮
    backBtn = new QPushButton(this);
    backBtn->move(170,200);
    backBtn->setText("返回");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("mintcream"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    //单击登录按钮时 执行 LoginForm::login 槽函数(自定义)；单击退出按钮时 执行 LoginForm::close 槽函数(窗体的关闭函数，不用自己写)
    connect(loginBtn,&QPushButton::clicked,this,&LoginForm::login);
    connect(backBtn,&QPushButton::clicked,this,&LoginForm::back);
    connect(reg,&QPushButton::clicked,this,&LoginForm::on_pushButton_clicked);
    connect(forgetpsw,&QPushButton::clicked,this,&LoginForm::on_pushButton_clicked2);
}

void LoginForm::login()
{
    //获得userNameLEd输入框的文本：userNameLEd->text()；
    //trimmed()去掉前后空格
    //tr()函数，防止设置中文时乱码
    QString name=this->userNameLEd->text();
    QString password=this->pwdLEd->text();
    this->userNameLEd->clear();
    this->pwdLEd->clear();
    QSqlQuery query;
    int record;
    if(type->currentText()=="管理员")
    {


        QString str=QString("select * from admin where userName='%1' and psw='%2'").arg(name).arg(password);
        query.exec(str);
        query.last();
        record=0;
        record=query.at()+1;

        if(record!=0)
        {
            this->hide();
            Admin* admin=new Admin;
            admin->show();
            connect(admin,&Admin::backToLogin,[=](){
            this->show();
            admin->hide();
            });
        }
        else
        {
            QMessageBox::warning(this, tr("警告！"),tr("用户名或密码错误！"),QMessageBox::Yes);
            // 清空输入框内容
               userNameLEd->clear();
               pwdLEd->clear();

               //光标定位
               userNameLEd->setFocus();
            }
        }



    else if(type->currentText()=="工作人员")
    {
        record=0;
        QString psw=QCryptographicHash::hash(password.toLocal8Bit(),QCryptographicHash::Md5).toHex();
        QString str=QString("select userName,posi from worker where id='%1' and psw='%2'").arg(name).arg(psw);
        //qDebug()<<str;
        query.exec(str);
        QString posi,userName;
        while(query.next())
        {
            userName=query.value(0).toString();
            posi=query.value(1).toString();
        }

        query.last();
        record=query.at()+1;
        if(record!=0)
        {
            this->hide();
            if(posi=="导游")
            {
                Guide* guide=new Guide;
                guide->show();
                Guide::name=userName;
                guide->setWindowTitle("欢迎光临 "+userName+"!");
                connect(guide,&Guide::backToLogin,[=](){
                this->show();
                guide->hide();
                });
            }
            else
            {
                Manager* manager=new Manager;
                manager->show();
                connect(manager,&Manager::backToLogin,[=](){
                this->show();
                manager->hide();
                });
            }

        }
        else
        {
            QMessageBox::warning(this, tr("警告！"),tr("用户名或密码错误！"),QMessageBox::Yes);
            // 清空输入框内容
               userNameLEd->clear();
               pwdLEd->clear();

               //光标定位
               userNameLEd->setFocus();
            }
        }



    else if(type->currentText()=="游客")
    {
        record=0;
        QString psw=QCryptographicHash::hash(password.toLocal8Bit(),QCryptographicHash::Md5).toHex();
        QString str=QString("select * from tourist where id='%1' and psw='%2'").arg(name).arg(psw);
        query.exec(str);

        QString userName;
        while(query.next()) userName=query.value(2).toString();
        query.last();
        record=query.at()+1;
        if(record!=0)
        {
            this->hide();
            Tourist* tourist=new Tourist;
            tourist->getTouristname(userName);
            tourist->getTouristId(name);
            tourist->show();
            tourist->setWindowTitle("欢迎光临 "+userName+"!");
            connect(tourist,&Tourist::backToLogin,[=](){
                   this->show();
                   tourist->hide();
               });
        }
        else
        {
            QMessageBox::warning(this, tr("警告！"),tr("用户名或密码错误！"),QMessageBox::Yes);
        // 清空输入框内容
           userNameLEd->clear();
           pwdLEd->clear();

           //光标定位
           userNameLEd->setFocus();
        }

 }

}


void LoginForm::back()
{
    emit this->backToMain();

}

//注册
void LoginForm::on_pushButton_clicked()
{
    this->hide();
    Reg_account *reg=new Reg_account(this);
    reg->show();
    connect(reg,&Reg_account::backToMain,[=](){
        this->show();
        reg->hide();
    });

}

//忘记密码
void LoginForm::on_pushButton_clicked2()
{
    this->hide();
    Forgetpsw *fpsw=new Forgetpsw;
    fpsw->show();

    connect(fpsw,&Forgetpsw::backToLogin,[=](){
        this->show();
        fpsw->hide();
    });
    //login.exec();
}

