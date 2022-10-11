#ifndef CONTACT_H
#define CONTACT_H

#include <QWidget>

namespace Ui {
class contact;
}

class contact : public QWidget
{
    Q_OBJECT

public:
    explicit contact(QWidget *parent = nullptr);
    void getTouristname(QString name);
    void getTouristID(QString id);
    QString name;
    QString id;
    ~contact();
signals:
    void backToSelect();
private slots:
    void on_back_btn_clicked();

    void on_contact_btn_clicked();

    void on_confirm_btn_clicked();

    void on_confirm_btn2_clicked();

private:
    Ui::contact *ui;
};

#endif // CONTACT_H
