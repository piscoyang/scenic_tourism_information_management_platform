#ifndef TOURIST_H
#define TOURIST_H

#include <QDialog>

namespace Ui {
class Tourist;
}

class Tourist : public QDialog
{
    Q_OBJECT

public:
    explicit Tourist(QWidget *parent = nullptr);
    void getTouristname(QString name);
    void getTouristId(QString id);
    QString name;
    QString id;
    ~Tourist();
signals:
    void backToLogin();
private slots:
    void on_ticketbuy_btn_clicked();

    void on_ticketreturn_btn_clicked();

    void on_consult_btn_clicked();

    void on_recommend_btn_clicked();

    void on_search_btn_clicked();

    void on_back_btn_clicked();

private:
    Ui::Tourist *ui;
};

#endif // TOURIST_H
