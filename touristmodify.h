#ifndef TOURISTMODIFY_H
#define TOURISTMODIFY_H

#include <QDialog>

namespace Ui {
class touristmodify;
}

class touristmodify : public QDialog
{
    Q_OBJECT
signals:
    void backToManage();
public:
    explicit touristmodify(QWidget *parent = nullptr);
    void guideSelectloading();
    ~touristmodify();

private slots:
    void on_Modify_clicked();

    void on_Delete_clicked();

    void on_back_clicked();

    void on_idinput_textChanged(const QString &arg1);

private:
    Ui::touristmodify *ui;
};

#endif // TOURISTMODIFY_H
