#ifndef SCENICMANAGEMENT_H
#define SCENICMANAGEMENT_H

#include <QWidget>

namespace Ui {
class Scenicmanagement;
}

class Scenicmanagement : public QWidget
{
    Q_OBJECT

public:
    explicit Scenicmanagement(QWidget *parent = nullptr);  /* 构造函数 */
    double Count_distanse(QPoint p_1, QPoint p_2);      /* 计算两点距离 */
    void All_flag_Clear();                              /* 清除flag */
    void Recover();                                     /* 按钮信息重置 */
    bool is_Click_Suc(QPoint x, QPoint y, int n);       /* 判断目标是否选中的函数 */
    bool Judge_HuiLu(int Degree[]);
    void DFS(int start, int n, int len);
    void Floyd();
    void Find_Shortest_Path( int start, int end );
    void loadfile(QString filename);
    void savemap(QString filename);
    ~Scenicmanagement();
signals:
    void backToManage();
    void backToMain();
private slots:
    void paintEvent(QPaintEvent*);              /* 窗口绘制 */
    void mousePressEvent(QMouseEvent* event);   /* 鼠标响应 */
    void on_Btn_1_1_clicked();                  /* 新增结点 */
    void on_Btn_1_2_clicked();                  /* 新增边 */
    void on_Btn_1_3_clicked();                  /* 删除结点 */
    void on_Btn_1_4_clicked();                  /* 删除边 */
    void on_Btn_1_5_clicked();                  /* 编辑结点信息 */
    void on_Btn_1_6_clicked();                  /* 修改边长 */
    void on_Btn_2_1_clicked();                  /* 判断图中有无回路 */
    void on_Btn_2_2_clicked();                  /* 建立导游线路图 */
    void on_Btn_2_3_clicked();                  /* 求两点之间的最短路径 */
    void on_Btn_2_4_clicked();                  /* 最小代价修建道路 */
    void on_Btn_3_1_clicked();                  /* 显示所有路径长 */
    void on_Btn_3_2_clicked();                  /* 加载地图 */
    void on_Btn_3_3_clicked();                  /* 保存地图 */
    void on_Btn_3_4_clicked();                  /* 加载背景 */
    void on_Btn_3_5_clicked();                  /* 清除屏幕 */
    void on_Btn_2_5_clicked();



    void on_scenic_info_clicked();

private:
    Ui::Scenicmanagement *ui;
};

#endif // SCENICMANAGEMENT_H
