#ifndef GUIDE_H
#define GUIDE_H

#include <QDialog>

namespace Ui {
class Guide;
}

class Guide : public QDialog
{
    Q_OBJECT
signals:
    void backToLogin();
public:
    explicit Guide(QWidget *parent = nullptr);
    ~Guide();
    static QString name;

private slots:

    void on_backbtn_clicked();

    void on_feedbackbtn_clicked();

    void on_managebtn_clicked();

private:
    Ui::Guide *ui;
};

#endif // GUIDE_H
