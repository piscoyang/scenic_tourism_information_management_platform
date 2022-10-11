#ifndef LEAVEMESSAGE_H
#define LEAVEMESSAGE_H

#include <QWidget>

namespace Ui {
class Leavemessage;
}

class Leavemessage : public QWidget
{
    Q_OBJECT

public:
    explicit Leavemessage(QWidget *parent = nullptr);
    ~Leavemessage();
signals:
    void backToSelect();
private slots:
    void on_pushButton_clicked();

private:
    Ui::Leavemessage *ui;
};

#endif // LEAVEMESSAGE_H
