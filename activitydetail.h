#ifndef ACTIVITYDETAIL_H
#define ACTIVITYDETAIL_H

#include <QWidget>

namespace Ui {
class activitydetail;
}

class activitydetail : public QWidget
{
    Q_OBJECT

public:
    explicit activitydetail(QWidget *parent = nullptr);
    void getActivityname(QString activityname,QString scenicname,QString gettime,QString introduction);
    void Lock(QString message);
    void scenicnameloading();
    ~activitydetail();
signals:
    void backToSelect();
private slots:
    void on_back_btn_clicked();

    void on_modify_btn_clicked();

    void on_add_btn_clicked();

    void on_delete_btn_clicked();

private:
    Ui::activitydetail *ui;
};

#endif // ACTIVITYDETAIL_H
