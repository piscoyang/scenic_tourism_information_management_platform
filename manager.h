#ifndef MANAGER_H
#define MANAGER_H

#include <QDialog>

namespace Ui {
class Manager;
}

class Manager : public QDialog
{
    Q_OBJECT
signals:
    void backToLogin();
public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();
private slots:
    void on_pushButton_clicked();

    void on_tourist_modify_btn_clicked();

    void on_contact_btn_clicked();

    void on_oder_lookup_clicked();

private:
    Ui::Manager *ui;
};

#endif // MANAGER_H
