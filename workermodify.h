#ifndef WORKERMODIFY_H
#define WORKERMODIFY_H

#include <QDialog>

namespace Ui {
class workermodify;
}

class workermodify : public QDialog
{
    Q_OBJECT
signals:
    void backToManage();
public:
    explicit workermodify(QWidget *parent = nullptr);
    ~workermodify();

private slots:
    void on_back_clicked();


    void on_Modify_clicked();

    void on_Delete_clicked();

    void on_Insert_clicked();


    void on_idinput_textChanged(const QString &arg1);

private:
    Ui::workermodify *ui;
    QString userId;
    QString name;
    QString sex;
    QString posi;
    QString psw;
};

#endif // WORKERMODIFY_H
