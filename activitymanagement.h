#ifndef ACTIVITYMANAGEMENT_H
#define ACTIVITYMANAGEMENT_H

#include <QWidget>

namespace Ui {
class activitymanagement;
}

class activitymanagement : public QWidget
{
    Q_OBJECT

public:
    explicit activitymanagement(QWidget *parent = nullptr);
    ~activitymanagement();
signals:
    void backToManage();
    void backToMain();
    void backToSelect();
private slots:
    void on_pushButton_clicked();

    void on_search_clicked();
    void modify();

    void on_add_btn_clicked();


private:
    Ui::activitymanagement *ui;
};

#endif // ACTIVITYMANAGEMENT_H
