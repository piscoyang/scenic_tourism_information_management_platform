#include "recommend.h"
#include "ui_recommend.h"
#include "loadmap.h"
#include <QtWidgets>        // 万能 Qt 头文件
#include <vector>
#include <fstream>
#include <algorithm>
#include <QSqlQuery>
Recommend::Recommend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Recommend)
{
    ui->setupUi(this);
    this->setWindowTitle("景区推荐系统");
    loadmap("F:/source/scenic_tourism_information_management_platform/scenic.My_map");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

Recommend::~Recommend()
{
    delete ui;
}

typedef struct line_struct	// 边的结构体
{
    int ind;                // 每条边的编号
    int node_1, node_2;     // 边的起点和终点
    double dis;             // 边的长度
    bool flag;              // 是否是当前标记为最短路径
}Line;                      // 起别名

const int show_window_x = 10;
const int show_window_y = 10;
const int show_window_width = 1340;      // 窗口的宽
const int show_window_height = 601;      // 窗口的高

const int Node_MAX_NUM = 100;            // 设置最大结点数
const int Side_MAX_NUM = 300;            // 设置最大边数
int node_num2;           // 结点个数
int side_num2;           // 边的条数
int info_ind2;           // 结点标签的数组下标

QPoint point2[Node_MAX_NUM];         // 结点数组
int HuiLu_point2[Node_MAX_NUM];      // 回路结点数组
Line line2[Side_MAX_NUM];            // 边数组
QString point2_info[Node_MAX_NUM];   // 结点的文字标签数组
QString point_message[Node_MAX_NUM]; //结点景点名称
Line temp_Line2;                     // 临时边
QPoint temp_point2_1, temp_point2_2;  // 两个临时的点

const int RADIUS = 5;               // 点击半径
int function_num2 = 0;               // 功能号(针对不同的操作会产生不同的效果)
QString pic_path2;                   // 图片路径
int HuiLu_flag2;                     // 构成回路的标志
int DFS_flag2;                       // DFS 路径的绘制标志
int Floyd_flag2;                     // 两点之间的最短路径的绘制标志
bool show_len_flag2 = false;         // 是否显示当前路径长度
double dis_matrix2[Node_MAX_NUM][Node_MAX_NUM];      // 距离矩阵
double D2[Node_MAX_NUM][Node_MAX_NUM];               // Floyd要用到的距离矩阵
int path2[Node_MAX_NUM][Node_MAX_NUM];               // 路径矩阵


double Recommend::Count_distanse(QPoint p_1, QPoint p_2)      // 计算两点间距离的函数
{
    double delta_x, delta_y;
    delta_x = p_1.x() - p_2.x();
    delta_y = p_1.y() - p_2.y();
    return sqrt( delta_x*delta_x + delta_y*delta_y );
}

void Recommend::All_flag_Clear()      // flag清除
{
    for(int i = 1; i <= side_num2; i++)
    {
        if(line2[i].flag)
            line2[i].flag = false;       // 红线变回蓝线
    }
    HuiLu_flag2 = 0;
    DFS_flag2 = 0;
    Floyd_flag2 = 0;
    update();                           // (重绘)调用 paintEvent 函数
}

void Recommend::Recover()             // 按钮信息重置
{
    ui->Message_1->setTextElideMode(Qt::ElideNone); // 自动换行

        ui->Message_1->clear();
        ui->Btn_2_2->setText("推荐线路1(道路不重复)");
        ui->Btn_2_3->setText("景点导航");
        ui->Btn_2_4->setText("推荐线路2（经过所有景点）");
        ui->Btn_2_5->setText("返回");

        ui->Btn_2_2->setStyleSheet("border-image: url(:/new/prefix1/btn_1.png);");
        ui->Btn_2_3->setStyleSheet("border-image: url(:/new/prefix1/btn_1.png);");
        ui->Btn_2_4->setStyleSheet("border-image: url(:/new/prefix1/btn_1.png);");
        ui->Btn_2_5->setStyleSheet("border-image: url(:/new/prefix1/btn_1.png);");
        ui->Btn_3_1->setStyleSheet("border-image: url(:/new/prefix1/btn_1.png);");



}

/*
 * 算法思路：
 * 假如我在屏幕上点击的位置为 x(QPoint属性), 目标点的位置在 y(也是QPoint属性)
 * 只要它俩的位置相差不大, 系统就会判断 “该目标点选中成功”
*/
bool Recommend::is_Click_Suc(QPoint x, QPoint y, int n)   // 判断目标是否选中的函数
{
    if (Count_distanse(x, y) <= n)
        return true;
    else
        return false;
}


/*
 * 算法思路：
 * [1] 首先设定每个结点最多只能被访问一次(有些结点所处的位置可能是“死胡同”)
 * [2] 用一维的 vector 来储存结果, 单元依次存储路径结点的 info.
 * [3] 采用DFS方式进行搜索, 每搜索到一个结点就放入当前的vector里面
 * [4] 发生递归(两种情况:<1>搜完了所有结点;<2>搜索失败), 注意对 vector 的调整
 * [5] 每次调用该函数时，都会进行“性能”比较，会将“好路线”更新到 ans 里面
 * [6] 最后输出对应 ans 最后一组结点信息即可。
*/
int vis2[Node_MAX_NUM];
int cur_max_node_num2 = 1;
int Shortest_path_len2 = 0;
int cnt2;
QVector<QString> vec_11;
QVector<QString> ans_11[500];
QVector<QPoint> vec_22;
QVector<QPoint> ans_22[500];
void Recommend::DFS(int start, int n, int len)     // start 为入口编号, n 为已加入路径的(景点)结点个数, len 为当前累加路经长
{
    if( n >= cur_max_node_num2 )
    {
        if( n > cur_max_node_num2)
        {
            cur_max_node_num2 = n;
            ans_11[cnt2] = vec_11;
            ans_22[cnt2] = vec_22;
            cnt2++;
        }
        else if( len < Shortest_path_len2 )
        {
            ans_11[cnt2] = vec_11;
            ans_22[cnt2] = vec_22;
            cnt2++;
        }
    }
    for( int i = 1; i <= node_num2; i++ )
    {
        if( dis_matrix2[start][i] != 0 && vis2[i] == 0 )  // 有路 + 未被访问过，那就可以下手了！
        {
            vis2[i] = 1;
            vec_11.push_back(point2_info[i]);
            vec_22.push_back(point2[i]);
            DFS(i, n+1, len+dis_matrix2[start][i]);
            vis2[i] = 0;
            vec_11.pop_back();
            vec_22.pop_back();
        }
    }
}

/*
 * 算法思路：
 * [1] 初始化距离矩阵和路径矩阵
 * [2] 依次加入每一个结点, 加入后更新距离矩阵和路径矩阵
 * [3] 更新机制：
 *      if( D2[i][k]+D2[k][j]<D2[i][j])
 *      {
 *          D2[i][j]=D2[i][k]+D2[k][j];    //更新矩阵
 *          path2[i][j]=path2[k][j];      //更新路径
 *      }
 * [4] 最后再进行 “回溯” 输出最短路径即可(详见Find_Shortest_Path函数)
*/
void Recommend::Floyd()
{
    for( int i = 1; i <= node_num2; i++ )
    {
        for( int j = 1; j <= node_num2; j++ )
        {
            if( dis_matrix2[i][j] == 0 )
                D2[i][j] = 999999;           // 这里用999999假装代表无穷大
            if( i == j )
                D2[i][i] = 0;                // 自己到自己的距离是 0
            if( i!=j && dis_matrix2[i][j] != 0)
            {
                D2[i][j] = dis_matrix2[i][j];
                path2[i][j] = i;             // 路径矩阵初始化
            }
        }
    }

    for( int k = 1; k <= node_num2; k++ )    // 每次新“解封”一个结点
    {
        for( int i = 1; i <= node_num2; i++ )
        {
            for( int j = 1; j <= node_num2; j++ )
            {
                if( D2[i][k] + D2[k][j] < D2[i][j] )
                {
                    D2[i][j] = D2[i][k] + D2[k][j];   // 动态更新距离矩阵
                    path2[i][j] = path2[k][j];       // 动态更新路径矩阵
                }
            }
        }
    }
}

void Recommend::Find_Shortest_Path( int start, int end )       // 通过回溯法找出最短路径
{
    vec_11.push_front(point2_info[end]);
    vec_22.push_front(point2[end]);
    while( start != end )
    {
        int add_ind = path2[start][end];
        vec_11.push_front(point2_info[add_ind]);
        vec_22.push_front(point2[add_ind]);
        end = path2[start][end];
    }
}

void drawArrow2(QPoint startpoint2, QPoint endpoint2, QPainter &p)         // 画带箭头的线
{
    double par = 15.0;                  // 箭头部分三角形的腰长
    double slopy = atan2((endpoint2.y() - startpoint2.y()), (endpoint2.x() - startpoint2.x()));
    double cosy = cos(slopy);
    double siny = sin(slopy);
    p.setBrush(Qt::red);                // 内部填充(白色)
    QPoint point21 = QPoint(endpoint2.x() + int(-par*cosy - (par / 2.0*siny)), endpoint2.y() + int(-par*siny + (par / 2.0*cosy)));
    QPoint point22 = QPoint(endpoint2.x() + int(-par*cosy + (par / 2.0*siny)), endpoint2.y() - int(par / 2.0*cosy + par*siny));
    QPoint point2s[3] = { endpoint2, point21, point22 };
    p.setRenderHint(QPainter::Antialiasing, true);  // 消锯齿
    QPen drawTrianglePen;                           // 创建画笔
    drawTrianglePen.setColor(Qt::red);
    drawTrianglePen.setWidth(1);
    p.setPen(drawTrianglePen);
    p.drawPolygon(point2s, 3);           // 绘制箭头部分
    int offsetX = int(par*siny / 3);
    int offsetY = int(par*cosy / 3);
    QPoint point23, point24;
    point23 = QPoint(endpoint2.x() + int(-par*cosy - (par / 2.0*siny)) + offsetX, endpoint2.y() + int(-par*siny + (par / 2.0*cosy)) - offsetY);
    point24 = QPoint(endpoint2.x() + int(-par*cosy + (par / 2.0*siny) - offsetX), endpoint2.y() - int(par / 2.0*cosy + par*siny) + offsetY);
    QPoint arrowBodypoint2s[3] = { startpoint2, point23, point24 };
    p.drawPolygon(arrowBodypoint2s, 3);  // 绘制箭身部分
}

void Recommend::paintEvent(QPaintEvent*)      // 绘制函数
{

    QPainter painter(this);                     // 画笔，this为当前窗口
    QFont font1("ZYSong 18030", 8);             // 新建 1 号字体
    QFont font2("ZYSong 18030", 10);            // 新建 2 号字体
    this->setMaximumSize(1361,789);             // 固定大小
    this->setMinimumSize(1361,789);
    painter.drawPixmap(0, 0, 1361, 789, QPixmap("end_Cover.png"));   // 大背景
    // painter.setBrush(Qt::white);            // 白底填充
    if( pic_path2 == "" )            // 如果无背景图片
        painter.setBrush(Qt::white);            // 白底填充
    else
    {
        painter.drawPixmap(show_window_x, show_window_y, show_window_width, show_window_height, QPixmap(pic_path2));   // 载入背景图片并显示出来
    }
    painter.drawRect(show_window_x, show_window_y, show_window_width, show_window_height);   // 绘制一个矩形框
    painter.setFont(font2);
    painter.setRenderHint(QPainter::Antialiasing, true);    // 设置:绘图光滑

    for( int i = 1; i <= side_num2; i++ )       // 先画线，更好看！不然线有覆盖“景点”的情况出现
    {
        if(!line2[i].flag)
        {
            painter.setPen(QPen(Qt::blue,2));           // 设置画笔颜色为蓝色
            painter.drawLine(point2[line2[i].node_1], point2[line2[i].node_2]);   //两点连线
        }
    }
    for( int i = 1; i <= side_num2; i++ )
    {
        if(line2[i].flag)
        {
            painter.setPen(QPen(Qt::red,4));            // 设置画笔颜色为红色(最小生成树)
            painter.drawLine(point2[line2[i].node_1], point2[line2[i].node_2]);   //两点连线
        }
    }

    if(DFS_flag2 == 1)
    {
        for( int i = 0 ; i < ans_22[cnt2-1].size()-1; i++ )           // 画 DFS 箭头
            drawArrow2(ans_22[cnt2-1][i], ans_22[cnt2-1][i+1], painter);
    }

    if(Floyd_flag2 == 1)
    {
        for( int i = 0 ; i < vec_22.size()-1; i++ )           // 画最短路径
            drawArrow2(vec_22[i], vec_22[i+1], painter);
    }

    painter.setFont(font1);
    if(show_len_flag2)        //如果显示边的标签
    {
        painter.setPen(Qt::darkMagenta);            // 洋红色
        for( int i = 1; i < node_num2; i++ )
        {
            for( int j = i + 1; j <= node_num2; j++ )
            {
                if( dis_matrix2[i][j] )             //若路径存在
                {
                    int zuo_x, zuo_y;
                    QString str = QString::number(dis_matrix2[i][j],'f', 1);    // 标签
                    zuo_x = (point2[i].x() + point2[j].x()) / 2 - 10;
                    zuo_y = (point2[i].y() + point2[j].y()) / 2 + 5;
                    painter.drawText( QPoint(zuo_x, zuo_y), str );      // 写在该“边”的中间偏左下的地方
                }
            }
        }
    }

    painter.setFont(font2);
    for(int i = 1; i <= node_num2; i++)
    {
        painter.setPen(Qt::black);      // 黑色
        painter.setBrush(Qt::yellow);     // 内部填充(黄色)
        painter.drawEllipse(point2[i], 4, 4);        // 把点画出来
        painter.drawText(QPoint(point2[i].x() + 5, point2[i].y() + 5), QString::number(i));    // 画出点的标签，point2_info[i]为标签
    }

    if(HuiLu_flag2 == 1)
    {
        for(int i = 1; i <= node_num2; i++)
        {

            int ind = HuiLu_point2[i];
            painter.setPen(Qt::black);      // 黑色
            painter.setBrush(Qt::red);      // 内部填充(红色)
            painter.drawEllipse(point2[ind], 4, 4);        // 把点画出来
            painter.drawText(QPoint(point2[i].x() + 5, point2[i].y() + 5), point2_info[i]);    // 画出点的标签，point2_info[i]为标签
        }
    }
}

void Recommend::mousePressEvent(QMouseEvent* e)   // 鼠标点击事件
{
    if (e->button() == Qt::LeftButton)              // 按左键
    {
        QPoint cur_click_pos = e->pos();            // e->pos(): 获取当前点击位置
        switch(function_num2)
        {

        case 1: // 添加点
            if(node_num2 < Node_MAX_NUM && cur_click_pos.x() >= show_window_x &&
                    cur_click_pos.x() <= show_window_x+show_window_width && cur_click_pos.y() >= show_window_y &&
                    cur_click_pos.y() <= show_window_y+show_window_height) // 判断所加的点是否在窗口范围内
            {
                int save_node_num2 = node_num2;
                node_num2++;
                for(int i = 1; i < node_num2; i++)
                {
                    if(is_Click_Suc(cur_click_pos, point2[i], RADIUS+10))   // 判断鼠标所点击位置和图上所有已添加的结点位置,是否靠的太近
                    {
                        node_num2--;
                        QMessageBox::warning(this, "警告", "两个点靠太近！");
                    }
                }
                if(save_node_num2 == node_num2)
                    break;
                point2[node_num2] = e->pos();                             // 当前位置赋给最新的结点
                point2_info[node_num2] = QString::number(++info_ind2);     // 创建默认标签
                update();
            }
            else if(node_num2 >= Node_MAX_NUM)
            {
                QMessageBox::warning(this, "警告", "目前结点数已达上限，无法再继续添加！");
            }
            else
            {
                QMessageBox::warning(this, "警告", "新加结点已超出边界！");
            }
            ui->Message_1->clear();
            ui->Message_1->addItem("目前有结点个数：" + QString::number(node_num2));
            ui->Message_1->addItem("目前有边的条数：" + QString::number(side_num2));
            ui->Message_1->addItem("如果还要继续添加, 请选择下一个点的位置。");
            function_num2 = 1;           // 功能号 1 保持不变(便于重复添加点)
            break;

        case 2:         // 新增边(选择边的起点)
            if(side_num2 >= Side_MAX_NUM)
            {
                QMessageBox::warning(this, "警告", "路径数已达上限！");
            }
            else
            {
                for( int i = 1; i <= node_num2; i++ )
                {
                    if( is_Click_Suc(cur_click_pos, point2[i], RADIUS) )   // 判断是否选中
                    {
                        function_num2 = 3;           // 找到了新增边的起点后, 还需找到其终点. 故把控制权交给功能号3
                        temp_point2_1 = point2[i];
                        line2[side_num2 + 1].node_1 = i;
                        ui->Message_1->clear();
                        ui->Message_1->addItem("请选择边的终点位置");
                        break;
                    }
                }
            }
            update();
            break;

        case 3:     // 新增边(选择边的终点)
            for( int i = 1; i <= node_num2; i++ )
            {
                if(point2[i] != temp_point2_1 && is_Click_Suc(cur_click_pos, point2[i], RADIUS)) // 若选中了与第一个点不同的点
                {
                    function_num2 = 2;                       // 重新把控制权交给功能号2(便于重复添加“边”)
                    int save_side_num2 = side_num2++;         // 线数量 + 1
                    temp_point2_2 = point2[i];
                    line2[side_num2].node_2 = i;
                    if(line2[side_num2].node_1 > line2[side_num2].node_2)        // 确保边的起点下标比终点的小, 不然做交换
                    {
                        int temp = line2[side_num2].node_1;
                        line2[side_num2].node_1 = line2[side_num2].node_2;
                        line2[side_num2].node_2 = temp;
                    }

                    for( int j = 1; j < side_num2; j++ )          // 判断是否路线已经存在
                    {
                        if(line2[side_num2].node_1 == line2[j].node_1 && line2[side_num2].node_2 == line2[j].node_2)
                        {
                            line2[side_num2] = line2[0];
                            side_num2--;
                            QMessageBox::warning(this, "警告", "该路径已添加！");
                            break;
                        }
                    }

                    if(save_side_num2 != side_num2)      // 如果路该线之前在图中不存在, 则该表达式成立
                    {
                        int ind_1 = line2[side_num2].node_1;
                        int ind_2 = line2[side_num2].node_2;
                        dis_matrix2[ind_1][ind_2] = dis_matrix2[ind_2][ind_1] = Count_distanse(point2[ind_1], point2[ind_2]);   // 距离矩阵赋值
                        line2[side_num2].ind = side_num2;      // 边的“编号”
                        line2[side_num2].dis = Count_distanse(point2[ind_1], point2[ind_2]);    // 边的长度
                    }
                    ui->Message_1->clear();
                    ui->Message_1->addItem("目前有结点个数：" + QString::number(node_num2));
                    ui->Message_1->addItem("目前有边的条数：" + QString::number(side_num2));
                    ui->Message_1->addItem("如果还要继续添加边, 请选择下一条边的起点");
                    break;
                }
            }
            update();
            break;

        case 4:                     // 删除结点
            if(node_num2 == 0)
            {
                QMessageBox::warning(this, "警告", "没有结点可删除！");
            }
            else
            {
                for( int i = 1; i <= node_num2; i++ )
                {
                    if(is_Click_Suc(cur_click_pos, point2[i], RADIUS))
                    {
                        for( int j = i; j < node_num2; j++ )
                        {
                            point2[j] = point2[j + 1];                        // 将删除的结点后的点前移
                            point2_info[j] = point2_info[j + 1];              // 结点的标签做同样处理
                        }

                        for( int j = i; j < node_num2; j++ )
                            for ( int k = 1; k <= node_num2; k++ )
                                dis_matrix2[k][j] = dis_matrix2[k][j + 1];    // 删除该结点所在的行

                        for( int j = i; j < node_num2; j++ )
                            for( int k = 1; k <= node_num2; k++ )
                                dis_matrix2[j][k] = dis_matrix2[j + 1][k];    // 删除该结点所在的列

                        for (int j = 1; j <= node_num2; j++)
                            dis_matrix2[j][node_num2] = dis_matrix2[node_num2][j] = 0;

                        point2[node_num2] = point2[0];                 // 最后一个点消失
                        point2_info[node_num2] = point2_info[0];       // 最后一个点标签消失(注意:其他点的标签保持不变)
                        node_num2--;

                        for( int j = 1; j <= side_num2; j++ )        // 对每一边都进行相应的判断
                        {
                            if(i == line2[j].node_1 || i == line2[j].node_2)   // 只要边的起点和终点有一个是“即将被删除的结点”,则进行后续处理
                            {
                                for( int k = j; k < side_num2; k++ )     // 将“边数组”也一并前移【除ind】
                                {
                                    line2[k].dis = line2[k + 1].dis;
                                    line2[k].flag = line2[k + 1].flag;
                                    line2[k].node_1 = line2[k + 1].node_1;
                                    line2[k].node_2 = line2[k + 1].node_2;
                                }
                                line2[side_num2] = line2[0];           // 将最后一条边去除
                                side_num2--;
                                j--;                // 连续判断所需(因为边数组已前移)
                            }
                            else                    // 在“边数组”里面, 也要更新每一条边(大于删除结点编)的起点和终点
                            {
                                if(line2[j].node_1 > i)
                                    line2[j].node_1--;
                                if(line2[j].node_2 > i)
                                    line2[j].node_2--;
                            }
                        }
                        update();
                        break;
                    }
                }
            }
            ui->Message_1->clear();
            ui->Message_1->addItem("目前有结点个数：" + QString::number(node_num2));
            ui->Message_1->addItem("目前有边的条数：" + QString::number(side_num2));
            ui->Message_1->addItem("如果还要继续删除结点, 请选择下一个点的位置。");
            function_num2 = 4;           // 功能号 4 保持不变(便于重复进行删除操作)
            break;

        case 5:         // 删除边(选择边的起点)
            if(side_num2 == 0)
            {
                QMessageBox::warning(this, "警告", "图中已无边可删除！");
            }
            else
            {
                for( int i = 1; i <= node_num2; i++ )
                {
                    if(is_Click_Suc(cur_click_pos, point2[i], RADIUS))
                    {
                        function_num2 = 6;           // 找到了删除边的起点后, 还需找到其终点. 故把控制权交给功能号6
                        temp_point2_1 = point2[i];
                        temp_Line2.node_1 = i;
                        ui->Message_1->addItem("请选择要需删除边的终点");
                        break;
                    }
                }
            }
            break;

        case 6:         // 删除边(选择边的终点)
            for(int i = 1; i <= node_num2; i++)
            {
                if(point2[i] != temp_point2_1 && is_Click_Suc(cur_click_pos, point2[i], RADIUS)) // 选中
                {
                    function_num2 = 5;                          // 重新把控制权交给功能号5(便于重复删除“边”)
                    temp_point2_2 = point2[i];
                    temp_Line2.node_2 = i;
                    if(temp_Line2.node_1 > temp_Line2.node_2)    // 保证线的起点的下标小于终点的下标
                    {
                        int temp = temp_Line2.node_1;
                        temp_Line2.node_1 = temp_Line2.node_2;
                        temp_Line2.node_2 = temp;
                    }
                    int side_save_num = side_num2;
                    for( int j = 1; j <= side_num2; j++ )
                    {
                        if(temp_Line2.node_1 == line2[j].node_1 && temp_Line2.node_2 == line2[j].node_2)   // 判断图中是否有该边
                        {
                            int start = line2[j].node_1;
                            int end = line2[j].node_2;
                            dis_matrix2[start][end] = dis_matrix2[end][start] = 0;        // 将距离矩阵中的该边清零
                            for (int k = j; k < side_num2; k++)      // 把(编号大于删除边的)下标前移【除ind】
                            {
                                line2[k].dis = line2[k + 1].dis;
                                line2[k].flag = line2[k + 1].flag;
                                line2[k].node_1 = line2[k + 1].node_1;
                                line2[k].node_2 = line2[k + 1].node_2;
                            }

                            line2[side_num2] = line2[0];
                            side_num2--;
                            break;
                        }
                    }
                    if(side_save_num == side_num2)
                    {
                        QMessageBox::warning(this, "警告", "图中无此边！");
                    }
                    else
                    {
                        ui->Message_1->clear();
                        ui->Message_1->addItem("目前有结点个数：" + QString::number(node_num2));
                        ui->Message_1->addItem("目前有边的条数：" + QString::number(side_num2));
                        ui->Message_1->addItem("如果还要继续删除边, 请选择下一条边的起点");
                        break;
                    }
                }
            }
            update();
            break;

        case 7:             // 编辑结点的标签
            if(node_num2 == 0)
            {
                QMessageBox::warning(this, "警告", "没有结点可编辑！");
            }
            else
            {
                for(int i = 1; i <= node_num2; i++)
                {
                    if(is_Click_Suc(cur_click_pos, point2[i], RADIUS))
                    {
                        QString str = QInputDialog::getText(this, "编辑框", "输入文本(最多15个字)");
                        if (str != "")
                            point2_info[i] = str.left(15);   // 左对齐
                        break;
                    }
                }
            }
            ui->Message_1->clear();
            ui->Message_1->addItem("如果还要继续编辑结点信息, 请选择下一个结点");
            break;

        case 8:     // 修改边(选择边的起点)
            if (side_num2 == 0)
            {
                QMessageBox::warning(this, "警告", "无任何路径");
            }
            else
            {
                for (int i = 1; i <= node_num2; i++)
                {
                    if (is_Click_Suc(cur_click_pos, point2[i], RADIUS))
                    {
                        function_num2 = 9;               // 找到了修改边的起点后, 还需找到其终点. 故把控制权交给功能号9
                        temp_point2_1 = point2[i];
                        temp_Line2.node_1 = i;
                        ui->Message_1->addItem("请选择需修改边的终点");
                        break;
                    }
                }
            }
            break;

        case 9:    // 修改边(选择边的终点)
            for (int i = 1; i <= node_num2; i++)
            {
                if (point2[i] != temp_point2_1 && is_Click_Suc(cur_click_pos, point2[i], RADIUS))  //不能和起点重合
                {
                    function_num2 = 8;                   // 重新把控制权交给功能号8(便于重复修改“边”)
                    temp_point2_2 = point2[i];
                    temp_Line2.node_2 = i;
                    if (temp_Line2.node_1 > temp_Line2.node_2)    // 确保修改边起点下标小于终点的下标
                    {
                        int temp = temp_Line2.node_1;
                        temp_Line2.node_1 = temp_Line2.node_2;
                        temp_Line2.node_2 = temp;
                    }
                    bool temp_flag = false;             // 临时标记(看是否已作修改)
                    for (int j = 1; j <= side_num2; j++)
                    {
                        if (temp_Line2.node_1 == line2[j].node_1 && temp_Line2.node_2 == line2[j].node_2)   // 判断图中有无此边
                        {
                            double dis = dis_matrix2[temp_Line2.node_1][temp_Line2.node_2];
                            double number = QInputDialog::getDouble(this, "修改边的长度", "输入浮点数(0.0001~999999)", dis, 0.0001, 999999, 4);
                            if(number)
                                dis_matrix2[temp_Line2.node_1][temp_Line2.node_2] = dis_matrix2[temp_Line2.node_2][temp_Line2.node_1] = number;   // 若输入了数，则边进行修改
                            temp_flag = true;
                            break;
                        }
                    }
                    if (!temp_flag)
                    {
                        QMessageBox::warning(this, "警告", "图中无此边！");
                    }
                    else
                    {
                        ui->Message_1->addItem("如果还要继续修改边, 请选择下一条边的起点");
                    }
                    update();
                    break;
                }
            }
            break;

        case 10:        // 建立导游路线图(DFS)
            if( !(side_num2 >= 1 && node_num2 >= 2) )
            {
                QMessageBox::warning(this, "警告", "图中元素不满足实现该功能的前提条件！");
                break;
            }
            /* 清零、清空操作 */
            for( int i = 0 ; i <= node_num2; i++ )   // 标记清零
                vis2[i] = 0;
            if(!vec_11.isEmpty())
                vec_11.clear();                            // 清空
            if(!vec_22.isEmpty())
                vec_22.clear();                            // 清空
            cnt2 = 0;
            for( int i = 0; i < 500; i++)
            {
                if(!ans_11[i].isEmpty())
                    ans_11[i].clear();
                if(!ans_22[i].isEmpty())
                    ans_22[i].clear();
            }
            cur_max_node_num2 = 1;
            DFS_flag2 = 0;
            /* 以上这部分很重要 */
            for (int i = 1; i <= node_num2; i++)
            {
                if (is_Click_Suc(cur_click_pos, point2[i], RADIUS))
                {
                    vec_11.push_back(point2_info[i]);
                    vec_22.push_back(point2[i]);
                    vis2[i] = 1;
                    Shortest_path_len2 = 0;
                    DFS(i, 1, 0);
                    QString str = "";
                    for( int i = 0; i < ans_11[cnt2-1].size(); i++ )
                    {
                        if( i == 0 )
                        {
                            str = "导游路线为：" + ans_11[cnt2-1][i];
                        }
                        else
                        {
                            str = str + "——>" + ans_11[cnt2-1][i];
                        }
                    }
                    ui->Message_1->addItem(str);
                    DFS_flag2 = 1;
                    update();
                    break;
                }
            }
            ui->Message_1->addItem("如果还要查看其他旅游路线, 请选择重新选择旅游路线的起点");
            break;

        case 11:        // 求两点之间的最短路径(Floyd)——选择起点
            if( !(side_num2 >= 1 && node_num2 >= 2) )
            {
                QMessageBox::warning(this, "警告", "图中元素不满足实现该功能的前提条件！");
                break;
            }
            /* 清零、清空操作 */
            for( int i = 0 ; i <= node_num2; i++ )   // 标记清零
                vis2[i] = 0;
            if(!vec_11.isEmpty())
                vec_11.clear();                            // 清空
            if(!vec_22.isEmpty())
                vec_22.clear();                            // 清空
            cnt2 = 0;
            for( int i = 0; i < 500; i++)
            {
                if(!ans_11[i].isEmpty())
                    ans_11[i].clear();
                if(!ans_22[i].isEmpty())
                    ans_22[i].clear();
            }
            cur_max_node_num2 = 1;
            DFS_flag2 = 0;
            Floyd_flag2 = 0;
            /* 以上这部分很重要 */
            for(int i = 1; i <= node_num2; i++)
            {
                if(is_Click_Suc(cur_click_pos, point2[i], RADIUS))
                {
                    ui->Message_1->addItem("请选择终点");
                    function_num2 = 12;              // 找到了起点后, 还需找到其终点. 故把控制权交给功能号12
                    temp_point2_1 = point2[i];
                    temp_Line2.node_1 = i;           // 起点下标保存
                    break;
                }
            }
            break;

        case 12:         // 求两点之间的最短路径(Floyd)——选择终点

            for(int i = 1; i <= node_num2; i++)
            {
                if(point2[i] != temp_point2_1 && is_Click_Suc(cur_click_pos, point2[i], RADIUS)) // 判断：终点不能与起点重合
                {
                    function_num2 = 11;                          // 重新把控制权交给功能号11(便于重复找查最短路径)
                    temp_point2_2 = point2[i];
                    temp_Line2.node_2 = i;                       // 终点下标保存
                    Find_Shortest_Path(temp_Line2.node_1, temp_Line2.node_2);
                    QString str = "";
                    for( int i = 0; i < vec_11.size(); i++ )
                    {
                        if( i == 0 )
                        {
                            str = str + vec_11[i];
                        }
                        else
                        {
                            str = str + "——>" + vec_11[i];
                        }
                    }
                    vec_11.clear();        // 清空
                    ui->Message_1->addItem("最短路径为：" + str);
                    ui->Message_1->addItem("如果还要查看其他最短路径, 请选择重新选择起点和终点");
                    Floyd_flag2 = 1;
                    update();
                }
                else if( point2[i] == temp_point2_1 && is_Click_Suc(cur_click_pos, point2[i], RADIUS))
                {
                    QMessageBox::warning(this, "警告", "起点与终点不能重合！");
                }
            }
            break;
        }   // end for if(鼠标左键点击成功)
    }   // end for swtch(...)
}   //  end for mousePressEvent(...)



/*
 * 算法思路：
 * 采用拓扑排序，先用一维数组存储所有结点的度(数组下标设为结点对应的序号)
 * [1] 每有一条边，与之相连的结点的度数就加 1
 * [2] 然后我们进行遍历，将度数为 1 的结点删去(与之相连的边也要删去)
 * [3] 数组也要随之更新，删去的那条边所相连的结点的度数 - 1
 * [4] 如果已没有符合条件的结点被删去，则跳到[5]，否则转向[2]
 * [5] 如果还有数组中还有数，那么打印出它们即是存在回路的几个结点。
 *
 * 时间复杂度：O(n^2)
*/
void Recommend::on_Btn_2_2_clicked()      // 建立一张导游线路图(DFS)
{
    All_flag_Clear();
    Recover();
    if(function_num2 != 10)
    {
        function_num2 = 10;
        ui->Btn_2_2->setStyleSheet("border-image: url(:/new/prefix1/btn_2.png);");
        ui->Btn_2_2->setText("停止该操作");
        ui->Message_1->addItem("请选择旅游路线的起点");
    }
    else
    {
        function_num2 = 0;
        ui->Btn_2_2->setText("建立导游线路图");
        ui->Message_1->clear();
    }
}

void Recommend::on_Btn_2_3_clicked()      // 求两点之间的最短路径(Floyd)
{
    All_flag_Clear();
    Recover();
    if(function_num2 != 11)
    {
        function_num2 = 11;
        ui->Btn_2_3->setStyleSheet("border-image: url(:/new/prefix1/btn_2.png);");
        ui->Btn_2_3->setText("停止该操作");
        Floyd();
        ui->Message_1->addItem("请选择起点");
    }
    else
    {
        function_num2 = 0;
        ui->Btn_2_3->setText("求两点之间的最短路径");
        ui->Message_1->clear();
    }
}

/*
 * 算法思路：
 * [1] 将所有的边提取出来, 然后按边长进行排序(快排+升序)
 * [2] 再将每一个结点都划分为一个单独的集合(初始化)
 * [3] 然后依次加边(按边长从小到大的顺序), 被连接在一起的结点归属到一个集合
 * [4] 在加边过程中，加了这条边若会形成回路就跳过这条边(采用查并集算法)
 * [5] 当所有的结点(n个)都被加入且在一个连通集合里面的时候, 把所有加入的边(n-1条)都输出出来, 图形界面展示即可
 * [6] 最后给出景区建设中的能花最小的代价的道路建设。
*/
int set2[Node_MAX_NUM+1];        //每个结点属于的集合
int cmp2( Line x, Line y )
{
    return x.dis < y.dis;
}

int Find2( int x )
{
    if( x==set2[x] )
        return x;
    return set2[x]=Find2(set2[x]);
}

void He_bing2( int x,int y )
{
    int fx=Find2(x),fy=Find2(y);
    set2[fx]=fy;
}

void Recommend::on_Btn_2_4_clicked()      // 最小生成树(Kruskal)
{

    All_flag_Clear();
    Recover();
    if(function_num2 != 14)
    {
        function_num2 = 14;
        ui->Btn_2_4->setStyleSheet("border-image: url(:/new/prefix1/btn_2.png);");
        ui->Btn_2_4->setText("停止该操作");
        using namespace std;
        /* 初始化操作 */
        for( int i = 1; i <= node_num2 ; i++ )
            set2[i] = i;
        /* 以上这部分很重要 */
        Line new_Line[Side_MAX_NUM+1];          // 重新开一个数组, 这样就不会破坏原来的数组

        for( int i = 0; i < side_num2; i++ )
            new_Line[i] = line2[i+1];

        std::sort(new_Line, new_Line+side_num2, cmp2);

        int i = 0, cnt2 = 0;
        QVector<int> vec;
        while( cnt2 != node_num2-1 )
        {
            if( i == side_num2 )
                break;
            int inital_ind = new_Line[i].ind;
            int node_ind_1 = line2[ inital_ind ].node_1;
            int node_ind_2 = line2[ inital_ind ].node_2;
            if( Find2(node_ind_1) != Find2(node_ind_2) )
            {
                vec.push_back( inital_ind );
                He_bing2(node_ind_1, node_ind_2);
                cnt2++;
            }
            i++;
        }

        if( cnt2 != node_num2-1 )
        {
            QMessageBox::warning(this, "警告", "该图不连通, 不存在最小代价生成树！");
        }
        else
        {
            for( int i = 0; i < node_num2-1; i++ )
            {
                line2[vec[i]].flag = 1;
            }
            QString str = "最小代价生成树生成成功！";
            ui->Message_1->addItem(str);
            ui->Btn_2_4->setText("关闭该功能");
            update();
        }
    }
    else
    {
        function_num2 = 0;
        ui->Btn_2_4->setText("推荐线路2（经过所有景点）");
        ui->Message_1->clear();
    }
}

void Recommend::on_Btn_3_1_clicked()          // 显示所有边长
{
    if(!show_len_flag2 )
    {
        show_len_flag2 = true;
        ui->Btn_3_1->setText("隐藏所有边的长");
        ui->Btn_3_1->setStyleSheet("border-image: url(:/new/prefix1/btn_2.png);");
    }
    else
    {
        show_len_flag2 = false;
        ui->Btn_3_1->setText("显示所有边的长");  // 每点一下切换一下状态
        ui->Btn_3_1->setStyleSheet("border-image: url(:/new/prefix1/btn_1.png);");
    }
    update();
}


void Recommend::loadmap(QString filename)
{
    using namespace std;
    QSqlQuery query;
    if(filename != "")      // 判断文件地址是否输入成功
    {
        //qDebug()<<filename<<endl;
        int x, y;           // 用于读取征信数据
        string str;         // 用于读取字符串数据
        ifstream in(filename.toStdString());
        if(in.is_open())
        {
            in >> side_num2 >> node_num2;
            for( int i = 1; i <= node_num2; i++ )
            {
                in >> x >> y;       // 读取每一个点的横纵坐标
                point2[i].setX(x);
                point2[i].setY(y);
                in >> str;
                point2_info[i] = QString::fromStdString(str);

            }

            for( int i = 1; i <= node_num2; i++ )
                for (int j = 1; j <= node_num2; j++)
                    in >> dis_matrix2[i][j];     // 读取距离矩阵

            for( int i = 1; i <= side_num2; i++ )
                in >> line2[i].ind >> line2[i].node_1 >> line2[i].node_2 >> line2[i].dis;   // 读取每条边的数据

            in >> str;          // 背景图片读取路径
            pic_path2 = QString::fromStdString(str);
            //QMessageBox::information(this, "提示", "读取地图成功");
            in.close();         // 关闭文件
            QString str="";
            for(int i = 1;i<= node_num2;i++)
            {

                str= QString("select name from scenic where id = %1").arg(point2_info[i]);
                //qDebug()<<str;
                query.exec(str);
                while(query.next())
                {
                    if(!query.value(0).toString().isEmpty())
                    point_message[i]=query.value(0).toString();
                    else
                        point_message[i]=QString::number(i);
                    //qDebug()<<"message"<<i<<point_message[i];
                }
               point2_info[i] = point_message[i];
            }
        }
        else
        {
            QMessageBox::information(this, "提示", "读取地图失败");
        }
    }
}






void Recommend::on_Btn_2_5_clicked()
{
    this->hide();
    emit this->backToMain();
}


void Recommend::on_Btn_2_6_clicked()
{

    LoadMap lm;
    lm.setWindowTitle("景区详细地图");
    lm.show();
    lm.exec();

}

