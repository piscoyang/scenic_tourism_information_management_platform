#ifndef TICKETBUY_H
#define TICKETBUY_H

#include <QWidget>

namespace Ui {
class TicketBuy;
}

class TicketBuy : public QWidget
{
    Q_OBJECT

public:
    explicit TicketBuy(QWidget *parent = nullptr);
    void guideSelectloading();
    void getTouristname(QString name);
    bool isQualified();
    QString name;
    ~TicketBuy();
signals:
    void backToSelect();
private slots:
    void on_back_btn_clicked();

    void on_consult_btn_clicked();

    void on_pushButton_clicked();


private:
    Ui::TicketBuy *ui;
};

#endif // TICKETBUY_H
