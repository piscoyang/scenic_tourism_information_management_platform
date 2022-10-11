#ifndef FORGETPSW_H
#define FORGETPSW_H

#include <QDialog>

namespace Ui {
class Forgetpsw;
}

class Forgetpsw : public QDialog
{
    Q_OBJECT

public:
    explicit Forgetpsw(QWidget *parent = nullptr);
    ~Forgetpsw();
signals:
    void backToLogin();
private slots:
    void on_backbtn_clicked();

    void on_confirm_clicked();

private:
    Ui::Forgetpsw *ui;
};

#endif // FORGETPSW_H
