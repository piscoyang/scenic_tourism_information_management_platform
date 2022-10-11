#ifndef TOURISTMANAGEMENT_H
#define TOURISTMANAGEMENT_H

#include <QDialog>

namespace Ui {
class touristmanagement;
}

class touristmanagement : public QDialog
{
    Q_OBJECT
signals:
    void backToLogin();
public:
    explicit touristmanagement(QWidget *parent = nullptr);
    ~touristmanagement();
    void getPriority(QString name,int a);
    QString static operator_name;

private slots:


    void on_back_clicked();

    void on_search_clicked();

    void on_modify_clicked();

private:
    Ui::touristmanagement *ui;
};

#endif // TOURISTMANAGEMENT_H
