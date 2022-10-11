#ifndef ORDER_INFORMATION_H
#define ORDER_INFORMATION_H

#include <QWidget>

namespace Ui {
class Order_information;
}

class Order_information : public QWidget
{
    Q_OBJECT

public:
    explicit Order_information(QWidget *parent = nullptr);
    ~Order_information();
    void getPriority(int a);
signals:
    void backToSelect();
private slots:

    void on_back_btn_clicked();


    void on_confirm_clicked();

    void on_confirm_2_clicked();

private:
    void orderloading();
    Ui::Order_information *ui;
};
#endif // ORDER_INFORMATION_H
