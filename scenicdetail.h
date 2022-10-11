#ifndef SCENICDETAIL_H
#define SCENICDETAIL_H

#include <QWidget>

namespace Ui {
class scenicdetail;
}

class scenicdetail : public QWidget
{
    Q_OBJECT

public:
    explicit scenicdetail(QWidget *parent = nullptr);
    void getData(int id,QString name,QString area);
    void getScenicData(QString scenicid,QString scenicname,QString scenicarea,QString scenicinfo);
    ~scenicdetail();
signals:
    void backToSelect();
private slots:
    void on_pushButton_clicked();
    void on_modify_btn_clicked();

private:
    Ui::scenicdetail *ui;
    QString id;
};

#endif // SCENICDETAIL_H
