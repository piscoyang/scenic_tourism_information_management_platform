#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>

namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();
signals:
    void backToLogin();
private slots:
    void on_exitbtn_clicked();

    void on_workermanage_clicked();

    void on_touristmanage_clicked();

    void on_scenicmanage_clicked();

    void on_activitymanage_clicked();

    void on_ordermanagement_btn_clicked();

private:
    Ui::Admin *ui;
};

#endif // ADMIN_H
