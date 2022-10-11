#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_login_btn_clicked();

    void on_register_btn_clicked();

    void on_search_btn_clicked();

    void on_recommend_btn_clicked();

    void on_exit_btn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
