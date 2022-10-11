#ifndef SCENICINFOEDIT_H
#define SCENICINFOEDIT_H

#include <QWidget>

namespace Ui {
class ScenicInfoEdit;
}

class ScenicInfoEdit : public QWidget
{
    Q_OBJECT

public:
    explicit ScenicInfoEdit(QWidget *parent = nullptr);
    ~ScenicInfoEdit();
signals:
    void backToPrevious();
private slots:
    void on_pushButton_clicked();

    void on_search_clicked();

    void modify();

private:
    Ui::ScenicInfoEdit *ui;
};

#endif // SCENICINFOEDIT_H
