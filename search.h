#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include <scenicdetail.h>
namespace Ui {
class Search;
}

class Search : public QDialog
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();
signals:
    void backToMain();
private slots:
    void on_pushButton_clicked();

    void on_backbtn_clicked();

    void introbtn_clicked();

    void detail();


private:
    Ui::Search *ui;
    scenicdetail *_interface = new scenicdetail;
};

#endif // SEARCH_H
