#ifndef REG_ACCOUNT_H
#define REG_ACCOUNT_H
#include <QMessageBox>
#include <QDialog>
namespace Ui {
class Reg_account;
}

class Reg_account : public QDialog
{
    Q_OBJECT

public:
    explicit Reg_account(QWidget *parent = nullptr);
    ~Reg_account();
signals:
    void backToMain();
private slots:



    void on_backbtn_clicked();

    void on_confirmbtn_clicked();

private:
    Ui::Reg_account *ui;
};


#endif // REG_ACCOUNT_H
