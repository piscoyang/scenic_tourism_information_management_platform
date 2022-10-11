#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>

class LoginForm : public QDialog
{
    Q_OBJECT //使用信号与槽需要的宏
public:
    explicit LoginForm(QDialog *parent = 0);  //explicit 防止歧义

signals:
    void backToMain();

public slots:
    void login();//点击登录按钮是执行的槽函数
    void back();
    void on_pushButton_clicked();
    void on_pushButton_clicked2();
private:
    QLabel *logintype;          //登录选项标签
    QComboBox *type;
    QLabel *userNameLbl;         //"用户名"标签
    QLabel *pwdLbl;              //"密码"标签
    QLineEdit *userNameLEd;      //用户名编辑行
    QLineEdit *pwdLEd;           //密码编辑行
    QPushButton *loginBtn;       //登录按钮
    QPushButton *backBtn;        //退出按钮
    QPushButton *forgetpsw;      //忘记密码
    QPushButton *reg;       //注册
};

#endif // LOGINFORM_H
