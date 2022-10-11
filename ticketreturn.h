#ifndef TICKETRETURN_H
#define TICKETRETURN_H

#include <QWidget>

namespace Ui {
class Ticketreturn;
}

class Ticketreturn : public QWidget
{
    Q_OBJECT

public:
    explicit Ticketreturn(QWidget *parent = nullptr);
    void getTouristname(QString name);
    QString name;
    ~Ticketreturn();
signals:
    void backToSelect();
private slots:
    void on_back_btn_clicked();

    void on_consult_btn_clicked();
    void on_pushButton_clicked();

private:
    Ui::Ticketreturn *ui;
};

#endif // TICKETRETURN_H
