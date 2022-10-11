#ifndef WORKERMANAGEMENT_H
#define WORKERMANAGEMENT_H

#include <QWidget>

namespace Ui {
class workermanagement;
}

class workermanagement : public QWidget
{
    Q_OBJECT
signals:
    void backToLogin();

public:
    explicit workermanagement(QWidget *parent = nullptr);
    ~workermanagement();

private slots:

    void on_backbtn_clicked();

    void on_confirmbtn_clicked();

    void on_modify_clicked();

private:
    Ui::workermanagement *ui;
};

#endif // WORKERMANAGEMENT_H
