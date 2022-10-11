#ifndef RECOMMEND_H
#define RECOMMEND_H

#include <QWidget>
#include <scenicmanagement.h>
namespace Ui {
class Recommend;
}

class Recommend : public QWidget
{
    Q_OBJECT

public:
    explicit Recommend(QWidget *parent = nullptr);
    double Count_distanse(QPoint p_1, QPoint p_2);      /* 计算两点距离 */
    void All_flag_Clear();                              /* 清除flag */
    void Recover();                                     /* 按钮信息重置 */
    bool is_Click_Suc(QPoint x, QPoint y, int n);       /* 判断目标是否选中的函数 */
    void DFS(int start, int n, int len);
    void Floyd();
    void Find_Shortest_Path( int start, int end );
    void loadmap(QString filename);
    ~Recommend();
signals:
    void backToManage();
    void backToMain();

private slots:
    void paintEvent(QPaintEvent*);              /* 窗口绘制 */
    void mousePressEvent(QMouseEvent* event);   /* 鼠标响应 */
    void on_Btn_2_5_clicked();

    void on_Btn_3_1_clicked();


    void on_Btn_2_2_clicked();

    void on_Btn_2_3_clicked();

    void on_Btn_2_4_clicked();

    void on_Btn_2_6_clicked();

private:
    Ui::Recommend *ui;
};

#endif // RECOMMEND_H
