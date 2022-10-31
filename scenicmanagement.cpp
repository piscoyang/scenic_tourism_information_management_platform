#include "scenicmanagement.h"
#include "ui_scenicmanagement.h"
#include "scenicinfoedit.h"
#include <QtWidgets>        // 万能 Qt 头文件
#include <vector>
#include <fstream>
#include <algorithm>
#include <QSqlQuery>
#include <QDebug>

Scenicmanagement::Scenicmanagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scenicmanagement)
{
    QString mapPath=QCoreApplication::applicationDirPath()+"/scenic.My_map";
    mapPath.replace(QString("/"),QString("\\"));
    ui->setupUi(this);
    setWindowTitle("景区景点、道路管理");

    loadfile(mapPath);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor("lightyellow"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

}

Scenicmanagement::~Scenicmanagement()
{
    delete ui;
}


typedef struct line_struct	// 道路的结构体
{
    int ind;                // 每条道路的编号
    int node_1, node_2;     // 道路的起点和终点
    double dis;             // 道路的长度
    bool flag;              // 是否是当前标记为最短路径
}Line;                      // 起别名

const int show_window_x = 20;
const int show_window_y = 60;
const int show_window_width = 1161;      // 窗口的宽
const int show_window_height = 511;      // 窗口的高

const int Node_MAX_NUM = 100;            // 设置最大景点数
const int Side_MAX_NUM = 300;            // 设置最大道路数
int node_num;           // 景点个数
int side_num;           // 道路的条数
int info_ind;           // 景点标签的数组下标

QPoint point[Node_MAX_NUM];         // 景点数组
int HuiLu_point[Node_MAX_NUM];      // 回路景点数组
Line line[Side_MAX_NUM];            // 道路数组
QString point_info[Node_MAX_NUM];   // 景点的文字标签数组
Line temp_Line;                     // 临时道路
QPoint temp_Point_1, temp_Point_2;  // 两个临时的点

const int RADIUS = 5;               // 点击半径
int function_num = 0;               // 功能号(针对不同的操作会产生不同的效果)

QString pic_path=":/new/prefix1/appimages/map.png";                   // 图片路径
int HuiLu_flag;                     // 构成回路的标志
int DFS_flag;                       // DFS 路径的绘制标志
int Floyd_flag;                     // 两点之间的最短路径的绘制标志
bool show_len_flag = false;         // 是否显示当前路径长度
double dis_matrix[Node_MAX_NUM][Node_MAX_NUM];      // 距离矩阵
double D[Node_MAX_NUM][Node_MAX_NUM];               // Floyd要用到的距离矩阵
int path[Node_MAX_NUM][Node_MAX_NUM];               // 路径矩阵


double Scenicmanagement::Count_distanse(QPoint p_1, QPoint p_2)      // 计算两点间距离的函数
{
    double delta_x, delta_y;
    delta_x = p_1.x() - p_2.x();
    delta_y = p_1.y() - p_2.y();
    return sqrt( delta_x*delta_x + delta_y*delta_y );
}

void Scenicmanagement::All_flag_Clear()      // flag清除
{
    for(int i = 1; i <= side_num; i++)
    {
        if(line[i].flag)
            line[i].flag = false;       // 红线变回蓝线
    }
    HuiLu_flag = 0;
    DFS_flag = 0;
    Floyd_flag = 0;
    update();                           // (重绘)调用 paintEvent 函数
}

void Scenicmanagement::Recover()             // 按钮信息重置
{
    ui->Message_1->setTextElideMode(Qt::ElideNone); // 自动换行

        ui->Message_1->clear();
        ui->Btn_1_1->setText("新增景点");
        ui->Btn_1_2->setText("新增道路");
        ui->Btn_1_3->setText("删除景点");
        ui->Btn_1_4->setText("删除道路");
        ui->Btn_1_5->setText("编辑景点名称");
        ui->Btn_1_6->setText("修改道路距离");
        ui->Btn_2_1->setText("判断图中有无回路");
        ui->Btn_2_2->setText("建立导游线路图");
        ui->Btn_2_3->setText("求两点之间的最短路径");
        ui->Btn_2_4->setText("最小代价修建道路");
        ui->Btn_2_5->setText("返回");

        ui->Btn_1_1->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_1_2->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_1_3->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_1_4->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_1_5->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_1_6->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_2_1->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_2_2->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_2_3->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_2_4->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_2_5->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_3_1->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_3_2->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_3_3->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_3_4->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
        ui->Btn_3_5->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");



}

/*
 * 算法思路：
 * 假如我在屏幕上点击的位置为 x(QPoint属性), 目标点的位置在 y(也是QPoint属性)
 * 只要它俩的位置相差不大, 系统就会判断 “该目标点选中成功”
*/
bool Scenicmanagement::is_Click_Suc(QPoint x, QPoint y, int n)   // 判断目标是否选中的函数
{
    if (Count_distanse(x, y) <= n)
        return true;
    else
        return false;
}


/*
 * 算法思路：
 * [1] 首先设定每个景点最多只能被访问一次(有些景点所处的位置可能是“死胡同”)
 * [2] 用一维的 vector 来储存结果, 单元依次存储路径景点的 info.
 * [3] 采用DFS方式进行搜索, 每搜索到一个景点就放入当前的vector里面
 * [4] 发生递归(两种情况:<1>搜完了所有景点;<2>搜索失败), 注意对 vector 的调整
 * [5] 每次调用该函数时，都会进行“性能”比较，会将“好路线”更新到 ans 里面
 * [6] 最后输出对应 ans 最后一组景点信息即可。
*/
int vis[Node_MAX_NUM];
int cur_max_node_num = 1;
int Shortest_path_len = 0;
int cnt;
QVector<QString> vec_1;
QVector<QString> ans_1[500];
QVector<QPoint> vec_2;
QVector<QPoint> ans_2[500];
void Scenicmanagement::DFS(int start, int n, int len)     // start 为入口编号, n 为已加入路径的(景点)景点个数, len 为当前累加路经长
{
    if( n >= cur_max_node_num )
    {
        if( n > cur_max_node_num)
        {
            cur_max_node_num = n;
            ans_1[cnt] = vec_1;
            ans_2[cnt] = vec_2;
            cnt++;
        }
        else if( len < Shortest_path_len )
        {
            ans_1[cnt] = vec_1;
            ans_2[cnt] = vec_2;
            cnt++;
        }
    }
    for( int i = 1; i <= node_num; i++ )
    {
        if( dis_matrix[start][i] != 0 && vis[i] == 0 )  // 有路 + 未被访问过，那就可以下手了！
        {
            vis[i] = 1;
            vec_1.push_back(point_info[i]);
            vec_2.push_back(point[i]);
            DFS(i, n+1, len+dis_matrix[start][i]);
            vis[i] = 0;
            vec_1.pop_back();
            vec_2.pop_back();
        }
    }
}

/*
 * 算法思路：
 * [1] 初始化距离矩阵和路径矩阵
 * [2] 依次加入每一个景点, 加入后更新距离矩阵和路径矩阵
 * [3] 更新机制：
 *      if( D[i][k]+D[k][j]<D[i][j])
 *      {
 *          D[i][j]=D[i][k]+D[k][j];    //更新矩阵
 *          path[i][j]=path[k][j];      //更新路径
 *      }
 * [4] 最后再进行 “回溯” 输出最短路径即可(详见Find_Shortest_Path函数)
*/
void Scenicmanagement::Floyd()
{
    for( int i = 1; i <= node_num; i++ )
    {
        for( int j = 1; j <= node_num; j++ )
        {
            if( dis_matrix[i][j] == 0 )
                D[i][j] = 999999;           // 这里用999999假装代表无穷大
            if( i == j )
                D[i][i] = 0;                // 自己到自己的距离是 0
            if( i!=j && dis_matrix[i][j] != 0)
            {
                D[i][j] = dis_matrix[i][j];
                path[i][j] = i;             // 路径矩阵初始化
            }
        }
    }

    for( int k = 1; k <= node_num; k++ )    // 每次新“解封”一个景点
    {
        for( int i = 1; i <= node_num; i++ )
        {
            for( int j = 1; j <= node_num; j++ )
            {
                if( D[i][k] + D[k][j] < D[i][j] )
                {
                    D[i][j] = D[i][k] + D[k][j];   // 动态更新距离矩阵
                    path[i][j] = path[k][j];       // 动态更新路径矩阵
                }
            }
        }
    }
}

void Scenicmanagement::Find_Shortest_Path( int start, int end )       // 通过回溯法找出最短路径
{
    vec_1.push_front(point_info[end]);
    vec_2.push_front(point[end]);
    while( start != end )
    {
        int add_ind = path[start][end];
        vec_1.push_front(point_info[add_ind]);
        vec_2.push_front(point[add_ind]);
        end = path[start][end];
    }
}

void drawArrow(QPoint startPoint, QPoint endPoint, QPainter &p)         // 画带箭头的线
{
    double par = 15.0;                  // 箭头部分三角形的腰长
    double slopy = atan2((endPoint.y() - startPoint.y()), (endPoint.x() - startPoint.x()));
    double cosy = cos(slopy);
    double siny = sin(slopy);
    p.setBrush(Qt::red);                // 内部填充(白色)
    QPoint point1 = QPoint(endPoint.x() + int(-par*cosy - (par / 2.0*siny)), endPoint.y() + int(-par*siny + (par / 2.0*cosy)));
    QPoint point2 = QPoint(endPoint.x() + int(-par*cosy + (par / 2.0*siny)), endPoint.y() - int(par / 2.0*cosy + par*siny));
    QPoint points[3] = { endPoint, point1, point2 };
    p.setRenderHint(QPainter::Antialiasing, true);  // 消锯齿
    QPen drawTrianglePen;                           // 创建画笔
    drawTrianglePen.setColor(Qt::red);
    drawTrianglePen.setWidth(1);
    p.setPen(drawTrianglePen);
    p.drawPolygon(points, 3);           // 绘制箭头部分
    int offsetX = int(par*siny / 3);
    int offsetY = int(par*cosy / 3);
    QPoint point3, point4;
    point3 = QPoint(endPoint.x() + int(-par*cosy - (par / 2.0*siny)) + offsetX, endPoint.y() + int(-par*siny + (par / 2.0*cosy)) - offsetY);
    point4 = QPoint(endPoint.x() + int(-par*cosy + (par / 2.0*siny) - offsetX), endPoint.y() - int(par / 2.0*cosy + par*siny) + offsetY);
    QPoint arrowBodyPoints[3] = { startPoint, point3, point4 };
    p.drawPolygon(arrowBodyPoints, 3);  // 绘制箭身部分
}

void Scenicmanagement::paintEvent(QPaintEvent*)      // 绘制函数
{

    QPainter painter(this);                     // 画笔，this为当前窗口
    QFont font1("ZYSong 18030", 8);             // 新建 1 号字体
    QFont font2("ZYSong 18030", 10);            // 新建 2 号字体
    this->setMaximumSize(1361,789);             // 固定大小
    this->setMinimumSize(1361,789);
    painter.drawPixmap(0, 0, 1361, 789, QPixmap("end_Cover.png"));   // 大背景
    // painter.setBrush(Qt::white);            // 白底填充
    if( pic_path == "" )            // 如果无背景图片
        painter.setBrush(Qt::white);            // 白底填充
    else
    {
        painter.drawPixmap(show_window_x, show_window_y, show_window_width, show_window_height, QPixmap(pic_path));   // 载入背景图片并显示出来
    }
    painter.drawRect(show_window_x, show_window_y, show_window_width, show_window_height);   // 绘制一个矩形框
    painter.setFont(font2);
    painter.setRenderHint(QPainter::Antialiasing, true);    // 设置:绘图光滑

    for( int i = 1; i <= side_num; i++ )       // 先画线，更好看！不然线有覆盖“景点”的情况出现
    {
        if(!line[i].flag)
        {
            painter.setPen(QPen(Qt::blue,2));           // 设置画笔颜色为蓝色
            painter.drawLine(point[line[i].node_1], point[line[i].node_2]);   //两点连线
        }
    }
    for( int i = 1; i <= side_num; i++ )
    {
        if(line[i].flag)
        {
            painter.setPen(QPen(Qt::red,4));            // 设置画笔颜色为红色(最小生成树)
            painter.drawLine(point[line[i].node_1], point[line[i].node_2]);   //两点连线
        }
    }

    if(DFS_flag == 1)
    {
        for( int i = 0 ; i < ans_2[cnt-1].size()-1; i++ )           // 画 DFS 箭头
            drawArrow(ans_2[cnt-1][i], ans_2[cnt-1][i+1], painter);
    }

    if(Floyd_flag == 1)
    {
        for( int i = 0 ; i < vec_2.size()-1; i++ )           // 画最短路径
            drawArrow(vec_2[i], vec_2[i+1], painter);
    }

    painter.setFont(font1);
    if(show_len_flag)        //如果显示道路的标签
    {
        painter.setPen(Qt::darkMagenta);            // 洋红色
        for( int i = 1; i < node_num; i++ )
        {
            for( int j = i + 1; j <= node_num; j++ )
            {
                if( dis_matrix[i][j] )             //若路径存在
                {
                    int zuo_x, zuo_y;
                    QString str = QString::number(dis_matrix[i][j],'f', 1);    // 标签
                    zuo_x = (point[i].x() + point[j].x()) / 2 - 10;
                    zuo_y = (point[i].y() + point[j].y()) / 2 + 5;
                    painter.drawText( QPoint(zuo_x, zuo_y), str );      // 写在该“道路”的中间偏左下的地方
                }
            }
        }
    }

    painter.setFont(font2);
    for(int i = 1; i <= node_num; i++)
    {
        painter.setPen(Qt::black);      // 黑色
        painter.setBrush(Qt::yellow);     // 内部填充(黄色)
        painter.drawEllipse(point[i], 4, 4);        // 把点画出来
        painter.drawText(QPoint(point[i].x() + 5, point[i].y() + 5), point_info[i]);    // 画出点的标签，point_info[i]为标签
    }

    if(HuiLu_flag == 1)
    {
        for(int i = 1; i <= node_num; i++)
        {

            int ind = HuiLu_point[i];
            painter.setPen(Qt::black);      // 黑色
            painter.setBrush(Qt::red);      // 内部填充(红色)
            painter.drawEllipse(point[ind], 4, 4);        // 把点画出来
            painter.drawText(QPoint(point[i].x() + 5, point[i].y() + 5), point_info[i]);    // 画出点的标签，point_info[i]为标签
        }
    }
}

void Scenicmanagement::mousePressEvent(QMouseEvent* e)   // 鼠标点击事件
{
    QString mapPath=QCoreApplication::applicationDirPath()+"/scenic.My_map";
    mapPath.replace(QString("/"),QString("\\"));
    if (e->button() == Qt::LeftButton)              // 按左键
    {
        QPoint cur_click_pos = e->pos();            // e->pos(): 获取当前点击位置
        switch(function_num)
        {

        case 1: // 添加点
            if(node_num < Node_MAX_NUM && cur_click_pos.x() >= show_window_x &&
                    cur_click_pos.x() <= show_window_x+show_window_width && cur_click_pos.y() >= show_window_y &&
                    cur_click_pos.y() <= show_window_y+show_window_height) // 判断所加的点是否在窗口范围内
            {
                int save_node_num = node_num;
                node_num++;
                for(int i = 1; i < node_num; i++)
                {
                    if(is_Click_Suc(cur_click_pos, point[i], RADIUS+10))   // 判断鼠标所点击位置和图上所有已添加的景点位置,是否靠的太近
                    {
                        node_num--;
                        QMessageBox::warning(this, "警告", "两个点靠太近！");
                    }
                }
                if(save_node_num == node_num)
                    break;
                point[node_num] = e->pos();                             // 当前位置赋给最新的景点
                point_info[node_num] = QString::number(++info_ind);     // 创建默认标签
                update();
                point_info[node_num]=QString::number(node_num);
                QSqlQuery query;
                query.exec(QString("insert into scenic VALUES(%1,'','亚洲区','');").arg(node_num));
            }
            else if(node_num >= Node_MAX_NUM)
            {
                QMessageBox::warning(this, "警告", "目前景点数已达上限，无法再继续添加！");
            }
            else
            {
                QMessageBox::warning(this, "警告", "新加景点已超出道路界！");
            }

            ui->Message_1->clear();
            ui->Message_1->addItem("目前有景点个数：" + QString::number(node_num));
            ui->Message_1->addItem("目前有道路的条数：" + QString::number(side_num));
            savemap(mapPath);
            ui->Message_1->addItem("如果还要继续添加, 请选择下一个点的位置。");
            function_num = 1;           // 功能号 1 保持不变(便于重复添加点)
            break;

        case 2:         // 新增道路(选择道路的起点)
            if(side_num >= Side_MAX_NUM)
            {
                QMessageBox::warning(this, "警告", "路径数已达上限！");
            }
            else
            {
                for( int i = 1; i <= node_num; i++ )
                {
                    if( is_Click_Suc(cur_click_pos, point[i], RADIUS) )   // 判断是否选中
                    {
                        function_num = 3;           // 找到了新增道路的起点后, 还需找到其终点. 故把控制权交给功能号3
                        temp_Point_1 = point[i];
                        line[side_num + 1].node_1 = i;
                        ui->Message_1->clear();
                        ui->Message_1->addItem("请选择道路的终点位置");
                        break;
                    }
                }
            }
            update();
            break;

        case 3:     // 新增道路(选择道路的终点)
            for( int i = 1; i <= node_num; i++ )
            {
                if(point[i] != temp_Point_1 && is_Click_Suc(cur_click_pos, point[i], RADIUS)) // 若选中了与第一个点不同的点
                {
                    function_num = 2;                       // 重新把控制权交给功能号2(便于重复添加“道路”)
                    int save_side_num = side_num++;         // 线数量 + 1
                    temp_Point_2 = point[i];
                    line[side_num].node_2 = i;
                    if(line[side_num].node_1 > line[side_num].node_2)        // 确保道路的起点下标比终点的小, 不然做交换
                    {
                        int temp = line[side_num].node_1;
                        line[side_num].node_1 = line[side_num].node_2;
                        line[side_num].node_2 = temp;
                    }

                    for( int j = 1; j < side_num; j++ )          // 判断是否路线已经存在
                    {
                        if(line[side_num].node_1 == line[j].node_1 && line[side_num].node_2 == line[j].node_2)
                        {
                            line[side_num] = line[0];
                            side_num--;
                            QMessageBox::warning(this, "警告", "该路径已添加！");
                            break;
                        }
                    }

                    if(save_side_num != side_num)      // 如果路该线之前在图中不存在, 则该表达式成立
                    {
                        int ind_1 = line[side_num].node_1;
                        int ind_2 = line[side_num].node_2;
                        dis_matrix[ind_1][ind_2] = dis_matrix[ind_2][ind_1] = Count_distanse(point[ind_1], point[ind_2]);   // 距离矩阵赋值
                        line[side_num].ind = side_num;      // 道路的“编号”
                        line[side_num].dis = Count_distanse(point[ind_1], point[ind_2]);    // 道路的长度
                    }
                    ui->Message_1->clear();
                    savemap(mapPath);
                    ui->Message_1->addItem("目前有景点个数：" + QString::number(node_num));
                    ui->Message_1->addItem("目前有道路的条数：" + QString::number(side_num));
                    ui->Message_1->addItem("如果还要继续添加道路, 请选择下一条道路的起点");
                    break;
                }
            }
            update();
            break;

        case 4:                     // 删除景点
            if(node_num == 0)
            {
                QMessageBox::warning(this, "警告", "没有景点可删除！");
            }
            else
            {
                QSqlQuery query;
                for( int i = 1; i <= node_num; i++ )
                {
                    if(is_Click_Suc(cur_click_pos, point[i], RADIUS))
                    {
                        query.exec(QString("delete from scenic where id=%1").arg(point_info[i]));
                        for( int j = i; j < node_num; j++ )
                        {
                            point[j] = point[j + 1];                        // 将删除的景点后的点前移
                            point_info[j] = point_info[j + 1];              // 景点的标签做同样处理
                        }

                        for( int j = i; j < node_num; j++ )
                            for ( int k = 1; k <= node_num; k++ )
                                dis_matrix[k][j] = dis_matrix[k][j + 1];    // 删除该景点所在的行

                        for( int j = i; j < node_num; j++ )
                            for( int k = 1; k <= node_num; k++ )
                                dis_matrix[j][k] = dis_matrix[j + 1][k];    // 删除该景点所在的列

                        for (int j = 1; j <= node_num; j++)
                            dis_matrix[j][node_num] = dis_matrix[node_num][j] = 0;

                        point[node_num] = point[0];                 // 最后一个点消失
                        point_info[node_num] = point_info[0];       // 最后一个点标签消失(注意:其他点的标签保持不变)
                        node_num--;

                        for( int j = 1; j <= side_num; j++ )        // 对每一道路都进行相应的判断
                        {
                            if(i == line[j].node_1 || i == line[j].node_2)   // 只要道路的起点和终点有一个是“即将被删除的景点”,则进行后续处理
                            {
                                for( int k = j; k < side_num; k++ )     // 将“道路数组”也一并前移【除ind】
                                {
                                    line[k].dis = line[k + 1].dis;
                                    line[k].flag = line[k + 1].flag;
                                    line[k].node_1 = line[k + 1].node_1;
                                    line[k].node_2 = line[k + 1].node_2;
                                }
                                line[side_num] = line[0];           // 将最后一条道路去除
                                side_num--;
                                j--;                // 连续判断所需(因为道路数组已前移)
                            }
                            else                    // 在“道路数组”里面, 也要更新每一条道路(大于删除景点编)的起点和终点
                            {
                                if(line[j].node_1 > i)
                                    line[j].node_1--;
                                if(line[j].node_2 > i)
                                    line[j].node_2--;
                            }
                        }
                        update();
                        break;
                    }
                }
            }
            ui->Message_1->clear();

            ui->Message_1->addItem("目前有景点个数：" + QString::number(node_num));
            ui->Message_1->addItem("目前有道路的条数：" + QString::number(side_num));
            ui->Message_1->addItem("如果还要继续删除景点, 请选择下一个点的位置。");
            savemap(mapPath);
            function_num = 4;           // 功能号 4 保持不变(便于重复进行删除操作)
            break;

        case 5:         // 删除道路(选择道路的起点)
            if(side_num == 0)
            {
                QMessageBox::warning(this, "警告", "图中已无道路可删除！");
            }
            else
            {
                for( int i = 1; i <= node_num; i++ )
                {
                    if(is_Click_Suc(cur_click_pos, point[i], RADIUS))
                    {
                        function_num = 6;           // 找到了删除道路的起点后, 还需找到其终点. 故把控制权交给功能号6
                        temp_Point_1 = point[i];
                        temp_Line.node_1 = i;
                        ui->Message_1->addItem("请选择要需删除道路的终点");
                        break;
                    }
                }
            }
            break;

        case 6:         // 删除道路(选择道路的终点)
            for(int i = 1; i <= node_num; i++)
            {
                if(point[i] != temp_Point_1 && is_Click_Suc(cur_click_pos, point[i], RADIUS)) // 选中
                {
                    function_num = 5;                          // 重新把控制权交给功能号5(便于重复删除“道路”)
                    temp_Point_2 = point[i];
                    temp_Line.node_2 = i;
                    if(temp_Line.node_1 > temp_Line.node_2)    // 保证线的起点的下标小于终点的下标
                    {
                        int temp = temp_Line.node_1;
                        temp_Line.node_1 = temp_Line.node_2;
                        temp_Line.node_2 = temp;
                    }
                    int side_save_num = side_num;
                    for( int j = 1; j <= side_num; j++ )
                    {
                        if(temp_Line.node_1 == line[j].node_1 && temp_Line.node_2 == line[j].node_2)   // 判断图中是否有该道路
                        {
                            int start = line[j].node_1;
                            int end = line[j].node_2;
                            dis_matrix[start][end] = dis_matrix[end][start] = 0;        // 将距离矩阵中的该道路清零
                            for (int k = j; k < side_num; k++)      // 把(编号大于删除道路的)下标前移【除ind】
                            {
                                line[k].dis = line[k + 1].dis;
                                line[k].flag = line[k + 1].flag;
                                line[k].node_1 = line[k + 1].node_1;
                                line[k].node_2 = line[k + 1].node_2;
                            }

                            line[side_num] = line[0];
                            side_num--;
                            break;
                        }
                    }
                    if(side_save_num == side_num)
                    {
                        QMessageBox::warning(this, "警告", "图中无此道路！");
                    }
                    else
                    {
                        ui->Message_1->clear();
                        savemap(mapPath);
                        ui->Message_1->addItem("目前有景点个数：" + QString::number(node_num));
                        ui->Message_1->addItem("目前有道路的条数：" + QString::number(side_num));
                        ui->Message_1->addItem("如果还要继续删除道路, 请选择下一条道路的起点");
                        break;
                    }
                }
            }
            update();
            break;

        case 7:             // 编辑景点的标签
            if(node_num == 0)
            {
                QMessageBox::warning(this, "警告", "没有景点可编辑！");
            }
            else
            {
                for(int i = 1; i <= node_num; i++)
                {
                    if(is_Click_Suc(cur_click_pos, point[i], RADIUS))
                    {
                        QString str = QInputDialog::getText(this, "编辑框", "输入文本(最多15个字)");
                        QString before;
                        if (str != "")
                            before=point_info[i];
                            point_info[i] = str.left(15);   // 左对齐
                        QSqlQuery query;
                        QString update=QString("update scenic set name ='%1' where name='%2'").arg(str).arg(before);
                        //qDebug()<<before;
                        query.exec(update);
                        break;
                    }
                }
            }
            ui->Message_1->clear();
            ui->Message_1->addItem("如果还要继续编辑景点信息, 请选择下一个景点");
            break;

        case 8:     // 修改道路(选择道路的起点)
            if (side_num == 0)
            {
                QMessageBox::warning(this, "警告", "无任何路径");
            }
            else
            {
                for (int i = 1; i <= node_num; i++)
                {
                    if (is_Click_Suc(cur_click_pos, point[i], RADIUS))
                    {
                        function_num = 9;               // 找到了修改道路的起点后, 还需找到其终点. 故把控制权交给功能号9
                        temp_Point_1 = point[i];
                        temp_Line.node_1 = i;
                        ui->Message_1->addItem("请选择需修改道路的终点");
                        break;
                    }
                }
            }
            break;

        case 9:    // 修改道路(选择道路的终点)
            for (int i = 1; i <= node_num; i++)
            {
                if (point[i] != temp_Point_1 && is_Click_Suc(cur_click_pos, point[i], RADIUS))  //不能和起点重合
                {
                    function_num = 8;                   // 重新把控制权交给功能号8(便于重复修改“道路”)
                    temp_Point_2 = point[i];
                    temp_Line.node_2 = i;
                    if (temp_Line.node_1 > temp_Line.node_2)    // 确保修改道路起点下标小于终点的下标
                    {
                        int temp = temp_Line.node_1;
                        temp_Line.node_1 = temp_Line.node_2;
                        temp_Line.node_2 = temp;
                    }
                    bool temp_flag = false;             // 临时标记(看是否已作修改)
                    for (int j = 1; j <= side_num; j++)
                    {
                        if (temp_Line.node_1 == line[j].node_1 && temp_Line.node_2 == line[j].node_2)   // 判断图中有无此道路
                        {
                            double dis = dis_matrix[temp_Line.node_1][temp_Line.node_2];
                            double number = QInputDialog::getDouble(this, "修改道路的长度", "输入浮点数(0.0001~999999)", dis, 0.0001, 999999, 4);
                            if(number)
                                dis_matrix[temp_Line.node_1][temp_Line.node_2] = dis_matrix[temp_Line.node_2][temp_Line.node_1] = number;   // 若输入了数，则道路进行修改
                            temp_flag = true;
                            break;
                        }
                    }
                    if (!temp_flag)
                    {
                        QMessageBox::warning(this, "警告", "图中无此道路！");
                    }
                    else
                    {
                        ui->Message_1->addItem("如果还要继续修改道路, 请选择下一条道路的起点");
                    }
                    update();
                    break;
                }
            }
            break;

        case 10:        // 建立导游路线图(DFS)
            if( !(side_num >= 1 && node_num >= 2) )
            {
                QMessageBox::warning(this, "警告", "图中元素不满足实现该功能的前提条件！");
                break;
            }
            /* 清零、清空操作 */
            for( int i = 0 ; i <= node_num; i++ )   // 标记清零
                vis[i] = 0;
            if(!vec_1.isEmpty())
                vec_1.clear();                            // 清空
            if(!vec_2.isEmpty())
                vec_2.clear();                            // 清空
            cnt = 0;
            for( int i = 0; i < 500; i++)
            {
                if(!ans_1[i].isEmpty())
                    ans_1[i].clear();
                if(!ans_2[i].isEmpty())
                    ans_2[i].clear();
            }
            cur_max_node_num = 1;
            DFS_flag = 0;
            /* 以上这部分很重要 */
            for (int i = 1; i <= node_num; i++)
            {
                if (is_Click_Suc(cur_click_pos, point[i], RADIUS))
                {
                    vec_1.push_back(point_info[i]);
                    vec_2.push_back(point[i]);
                    vis[i] = 1;
                    Shortest_path_len = 0;
                    DFS(i, 1, 0);
                    QString str = "";
                    for( int i = 0; i < ans_1[cnt-1].size(); i++ )
                    {
                        if( i == 0 )
                        {
                            str = "导游路线为：" + ans_1[cnt-1][i];
                        }
                        else
                        {
                            str = str + "——>" + ans_1[cnt-1][i];
                        }
                    }
                    ui->Message_1->addItem(str);
                    DFS_flag = 1;
                    update();
                    break;
                }
            }
            ui->Message_1->addItem("如果还要查看其他旅游路线, 请选择重新选择旅游路线的起点");
            break;

        case 11:        // 求两点之间的最短路径(Floyd)——选择起点
            if( !(side_num >= 1 && node_num >= 2) )
            {
                QMessageBox::warning(this, "警告", "图中元素不满足实现该功能的前提条件！");
                break;
            }
            /* 清零、清空操作 */
            for( int i = 0 ; i <= node_num; i++ )   // 标记清零
                vis[i] = 0;
            if(!vec_1.isEmpty())
                vec_1.clear();                            // 清空
            if(!vec_2.isEmpty())
                vec_2.clear();                            // 清空
            cnt = 0;
            for( int i = 0; i < 500; i++)
            {
                if(!ans_1[i].isEmpty())
                    ans_1[i].clear();
                if(!ans_2[i].isEmpty())
                    ans_2[i].clear();
            }
            cur_max_node_num = 1;
            DFS_flag = 0;
            Floyd_flag = 0;
            /* 以上这部分很重要 */
            for(int i = 1; i <= node_num; i++)
            {
                if(is_Click_Suc(cur_click_pos, point[i], RADIUS))
                {
                    ui->Message_1->addItem("请选择终点");
                    function_num = 12;              // 找到了起点后, 还需找到其终点. 故把控制权交给功能号12
                    temp_Point_1 = point[i];
                    temp_Line.node_1 = i;           // 起点下标保存
                    break;
                }
            }
            break;

        case 12:         // 求两点之间的最短路径(Floyd)——选择终点

            for(int i = 1; i <= node_num; i++)
            {
                if(point[i] != temp_Point_1 && is_Click_Suc(cur_click_pos, point[i], RADIUS)) // 判断：终点不能与起点重合
                {
                    function_num = 11;                          // 重新把控制权交给功能号11(便于重复找查最短路径)
                    temp_Point_2 = point[i];
                    temp_Line.node_2 = i;                       // 终点下标保存
                    Find_Shortest_Path(temp_Line.node_1, temp_Line.node_2);
                    QString str = "";
                    for( int i = 0; i < vec_1.size(); i++ )
                    {
                        if( i == 0 )
                        {
                            str = str + vec_1[i];
                        }
                        else
                        {
                            str = str + "——>" + vec_1[i];
                        }
                    }
                    vec_1.clear();        // 清空
                    ui->Message_1->addItem("最短路径为：" + str);
                    ui->Message_1->addItem("如果还要查看其他最短路径, 请选择重新选择起点和终点");
                    Floyd_flag = 1;
                    update();
                }
                else if( point[i] == temp_Point_1 && is_Click_Suc(cur_click_pos, point[i], RADIUS))
                {
                    QMessageBox::warning(this, "警告", "起点与终点不能重合！");
                }
            }
            break;
        }   // end for if(鼠标左键点击成功)
    }   // end for swtch(...)
}   //  end for mousePressEvent(...)

void Scenicmanagement::on_Btn_1_1_clicked()       // 新增景点
{
    All_flag_Clear();
    Recover();
    if(function_num != 1)
    {
        function_num = 1;
        ui->Btn_1_1->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
        ui->Btn_1_1->setText("停止该操作");
        ui->Message_1->clear();
        ui->Message_1->addItem("请选择一个位置添加新景点");
    }
    else
    {
        function_num = 0;
        ui->Btn_1_1->setText("新增景点");
        ui->Message_1->clear();
    }

}

void Scenicmanagement::on_Btn_1_2_clicked()      // 新增道路
{
    All_flag_Clear();
    Recover();
    if(function_num != 2)
    {
        function_num = 2;
        ui->Btn_1_2->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
        ui->Btn_1_2->setText("停止该操作");
        ui->Message_1->clear();
        ui->Message_1->addItem("请选择新增道路的起点");
    }
    else
    {
        function_num = 0;
        ui->Btn_1_2->setText("新增道路");
        ui->Message_1->clear();
    }
}

void Scenicmanagement::on_Btn_1_3_clicked()      // 删除景点
{
    All_flag_Clear();
    Recover();
    if(function_num != 4)
    {
        function_num = 4;
        ui->Btn_1_3->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
        ui->Btn_1_3->setText("停止该操作");
        ui->Message_1->clear();
        ui->Message_1->addItem("请选择删除景点的位置");
    }
    else
    {
        function_num = 0;
        ui->Btn_1_3->setText("删除景点");
        ui->Message_1->clear();
    }
}

void Scenicmanagement::on_Btn_1_4_clicked()      // 删除道路
{
    All_flag_Clear();
    Recover();
    if(function_num != 5)
    {
        function_num = 5;
        ui->Btn_1_4->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
        ui->Btn_1_4->setText("停止该操作");
        ui->Message_1->addItem("请选择需删除道路的起点");
    }
    else
    {
        function_num = 0;
        ui->Btn_1_4->setText("删除道路");
        ui->Message_1->clear();
    }
}

void Scenicmanagement::on_Btn_1_5_clicked()      // 编辑景点
{
    All_flag_Clear();
    Recover();
    if(function_num != 7)
    {
        function_num = 7;
        ui->Btn_1_5->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
        ui->Btn_1_5->setText("停止该操作");
        ui->Message_1->addItem("请选择需编辑的景点");
    }
    else
    {
        function_num = 0;
        ui->Btn_1_5->setText("编辑景点名称");
        ui->Message_1->clear();
    }
}

void Scenicmanagement::on_Btn_1_6_clicked()      // 修改道路
{
    All_flag_Clear();
    Recover();
    if(function_num != 8)
    {

        function_num = 8;
        ui->Btn_1_6->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
        ui->Btn_1_6->setText("停止该操作");
        ui->Message_1->addItem("请选择需要修改的道路的起点");
    }
    else
    {
        function_num = 0;
        ui->Btn_1_6->setText("修改道路距离");
        ui->Message_1->clear();
    }
}

/*
 * 算法思路：
 * 采用拓扑排序，先用一维数组存储所有景点的度(数组下标设为景点对应的序号)
 * [1] 每有一条道路，与之相连的景点的度数就加 1
 * [2] 然后我们进行遍历，将度数为 1 的景点删去(与之相连的道路也要删去)
 * [3] 数组也要随之更新，删去的那条道路所相连的景点的度数 - 1
 * [4] 如果已没有符合条件的景点被删去，则跳到[5]，否则转向[2]
 * [5] 如果还有数组中还有数，那么打印出它们即是存在回路的几个景点。
 *
 * 时间复杂度：O(n^2)
*/
bool Scenicmanagement::Judge_HuiLu(int Degree[])
{
    /* 初始化操作 */
    for( int i = 0; i <= node_num ; i++ )
        HuiLu_point[i] = 0;
    HuiLu_flag = 0;
    /* 以上这部分很重要 */

    for(int i = 1; i <= node_num; i++)
    {
        Degree[i] = 0;
    }

    for(int i = 1; i <= node_num; i++)
    {
        for(int j = i+1 ; j <= node_num; j++)
        {
            if( dis_matrix[i][j] != 0 )
            {
                Degree[i]++;
                Degree[j]++;
            }
        }
    }
    int Btn_2_1_flag = 1;
    int node_num_record = 0;
    while( Btn_2_1_flag )
    {
        Btn_2_1_flag = 0;               // 如果没有满足条件的景点存在则跳出循环
        for( int i = 1; i <= node_num; i++ )
        {
            if( Degree[i] == 1 )        // 每轮循环只对度数为 1 的景点做处理
            {
                Degree[i] = 0;
                Btn_2_1_flag = 1;
                node_num_record++;
                for( int j = 1; j <= node_num; j++ )
                {
                    if( dis_matrix[i][j] != 0 )
                    {
                        Degree[j]--;
                        break;
                    }
                }
            }
        }
    }
    if( node_num_record != node_num )
        return true;
    else
        return false;
}

void Scenicmanagement::on_Btn_2_1_clicked()      // 判断是否有回路
{

    All_flag_Clear();
    Recover();
    if(function_num != 13)
    {
        function_num = 13;
        ui->Btn_2_1->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
        ui->Btn_2_1->setText("停止该操作");
        int Degree[Node_MAX_NUM+1];
        if( Judge_HuiLu(Degree) )
        {
            QString str = "";
            int first = 0;
            int cnt = 1;
            for( int i = 1; i <= node_num ; i++ )
            {

                if( Degree[i] > 0 && first == 0 )
                {
                    str = str + point_info[i];
                    first = 1;
                    HuiLu_point[cnt++] = i;
                }
                else if( Degree[i] > 0 && first != 0 )
                {
                    str = str + "，" + point_info[i];
                    HuiLu_point[cnt++] = i;
                }
            }
            str = "存在回路的景点(景点标签为)：" + str;
            HuiLu_flag = 1;
            ui->Message_1->addItem(str);
        }
        else
        {
            function_num = 0;
            ui->Message_1->addItem("该图中无回路");
        }
    }
    else
    {
        function_num = 0;
        ui->Btn_2_1->setText("判断是否有回路");
        ui->Message_1->clear();
    }
}

void Scenicmanagement::on_Btn_2_2_clicked()      // 建立一张导游线路图(DFS)
{
    All_flag_Clear();
    Recover();
    if(function_num != 10)
    {
        function_num = 10;
        ui->Btn_2_2->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
        ui->Btn_2_2->setText("停止该操作");
        ui->Message_1->addItem("请选择旅游路线的起点");
    }
    else
    {
        function_num = 0;
        ui->Btn_2_2->setText("建立导游线路图");
        ui->Message_1->clear();
    }
}

void Scenicmanagement::on_Btn_2_3_clicked()      // 求两点之间的最短路径(Floyd)
{
    All_flag_Clear();
    Recover();
    if(function_num != 11)
    {
        function_num = 11;
        ui->Btn_2_3->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
        ui->Btn_2_3->setText("停止该操作");
        Floyd();
        ui->Message_1->addItem("请选择起点");
    }
    else
    {
        function_num = 0;
        ui->Btn_2_3->setText("求两点之间的最短路径");
        ui->Message_1->clear();
    }
}

/*
 * 算法思路：
 * [1] 将所有的道路提取出来, 然后按道路长进行排序(快排+升序)
 * [2] 再将每一个景点都划分为一个单独的集合(初始化)
 * [3] 然后依次加道路(按道路长从小到大的顺序), 被连接在一起的景点归属到一个集合
 * [4] 在加道路过程中，加了这条道路若会形成回路就跳过这条道路(采用查并集算法)
 * [5] 当所有的景点(n个)都被加入且在一个连通集合里面的时候, 把所有加入的道路(n-1条)都输出出来, 图形界面展示即可
 * [6] 最后给出景区建设中的能花最小的代价的道路建设。
*/
int set[Node_MAX_NUM+1];        //每个景点属于的集合
int cmp( Line x, Line y )
{
    return x.dis < y.dis;
}

int Find( int x )
{
    if( x==set[x] )
        return x;
    return set[x]=Find(set[x]);
}

void He_bing( int x,int y )
{
    int fx=Find(x),fy=Find(y);
    set[fx]=fy;
}

void Scenicmanagement::on_Btn_2_4_clicked()      // 最小生成树(Kruskal)
{

    All_flag_Clear();
    Recover();
    if(function_num != 14)
    {
        function_num = 14;
        ui->Btn_2_4->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
        ui->Btn_2_4->setText("停止该操作");
        using namespace std;
        /* 初始化操作 */
        for( int i = 1; i <= node_num ; i++ )
            set[i] = i;
        /* 以上这部分很重要 */
        Line new_Line[Side_MAX_NUM+1];          // 重新开一个数组, 这样就不会破坏原来的数组

        for( int i = 0; i < side_num; i++ )
            new_Line[i] = line[i+1];

        std::sort(new_Line, new_Line+side_num, cmp);

        int i = 0, cnt = 0;
        QVector<int> vec;
        while( cnt != node_num-1 )
        {
            if( i == side_num )
                break;
            int inital_ind = new_Line[i].ind;
            int node_ind_1 = line[ inital_ind ].node_1;
            int node_ind_2 = line[ inital_ind ].node_2;
            if( Find(node_ind_1) != Find(node_ind_2) )
            {
                vec.push_back( inital_ind );
                He_bing(node_ind_1, node_ind_2);
                cnt++;
            }
            i++;
        }

        if( cnt != node_num-1 )
        {
            QMessageBox::warning(this, "警告", "该图不连通, 不存在最小代价生成树！");
        }
        else
        {
            for( int i = 0; i < node_num-1; i++ )
            {
                line[vec[i]].flag = 1;
            }
            QString str = "最小代价生成树生成成功！";
            ui->Message_1->addItem(str);
            ui->Btn_2_4->setText("关闭该功能");
            update();
        }
    }
    else
    {
        function_num = 0;
        ui->Btn_2_4->setText("最小代价修建道路");
        ui->Message_1->clear();
    }
}

void Scenicmanagement::on_Btn_3_1_clicked()          // 显示所有道路长
{
    if(!show_len_flag )
    {
        show_len_flag = true;
        ui->Btn_3_1->setText("隐藏所有道路的长");
        ui->Btn_3_1->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
    }
    else
    {
        show_len_flag = false;
        ui->Btn_3_1->setText("显示所有道路的长");  // 每点一下切换一下状态
        ui->Btn_3_1->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
    }
    update();
}

void Scenicmanagement::loadfile(QString filename)
{
    using namespace std;
    if(filename != "")      // 判断文件地址是否输入成功
    {
        int x, y;           // 用于读取征信数据
        string str;         // 用于读取字符串数据
        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        std::string file = code->fromUnicode(filename).data();
        ifstream in(file);
        if(in.is_open())
        {
            in >> side_num >> node_num;
            for( int i = 1; i <= node_num; i++ )
            {
                in >> x >> y;       // 读取每一个点的横纵坐标
                point[i].setX(x);
                point[i].setY(y);
                in >> str;
                point_info[i] = QString::fromStdString(str);
            }

            for( int i = 1; i <= node_num; i++ )
                for (int j = 1; j <= node_num; j++)
                    in >> dis_matrix[i][j];     // 读取距离矩阵

            for( int i = 1; i <= side_num; i++ )
                in >> line[i].ind >> line[i].node_1 >> line[i].node_2 >> line[i].dis;   // 读取每条道路的数据

            in >> str;          // 背景图片读取路径
            pic_path = QString::fromStdString(str);
            //QMessageBox::information(this, "提示", "读取地图成功");
            in.close();         // 关闭文件
        }
        else
        {
            QMessageBox::information(this, "提示", "读取地图失败");
        }
    }
}
void Scenicmanagement::on_Btn_3_2_clicked()      // 加载地图
{

    All_flag_Clear();
    Recover();
    ui->Btn_3_2->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
    using namespace std;
    QString filename = QFileDialog::getOpenFileName(this, "选择文件", "", "地图文件(*.My_map)");
    loadfile(filename);
    ui->Btn_3_2->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
}

void Scenicmanagement::savemap(QString filename=QCoreApplication::applicationDirPath()+"/scenic.My_map")
{
    using namespace std;
    filename.replace(QString("/"),QString("\\"));
    if(filename != "")         // 判断文件地址是否输入成功
    {
        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        std::string file = code->fromUnicode(filename).data();
        ofstream out(file);               // 转换为标准字符串并进行保存
        if(out.is_open())
        {
            out << side_num << ' ' << node_num << endl;     // 先输入道路的条数和景点个数
            for( int i = 1; i <= node_num; i++ )
                out << point[i].x() << ' ' << point[i].y() << ' ' << point_info[i].toStdString() << endl;

            for( int i = 1; i <= node_num; i++ )
            {
                for( int j = 1; j <= node_num; j++ )
                    out << dis_matrix[i][j] << ' ';
                out << endl;
            }

            for( int i = 1; i <= side_num; i++ )
                out << line[i].ind << ' ' << line[i].node_1 << ' ' << line[i].node_2 << ' ' << line[i].dis << endl;
            out << pic_path.toStdString() << endl;

            QMessageBox::information(this, "提示", "保存地图成功");
            out.close();        // 关闭文档
        }
        else
            QMessageBox::information(this, "提示", "保存地图失败");
    }
}
void Scenicmanagement::on_Btn_3_3_clicked()      // 保存地图
{
    using namespace std;
    All_flag_Clear();
    Recover();
    ui->Btn_3_3->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", "", "地图文件(*.My_map)");  // 文件扩展名为.My_map
    if(filename != "")         // 判断文件地址是否输入成功
    {
        ofstream out(filename.toStdString());               // 转换为标准字符串并进行保存
        if(out.is_open())
        {
            out << side_num << ' ' << node_num << endl;     // 先输入道路的条数和景点个数
            for( int i = 1; i <= node_num; i++ )
                out << point[i].x() << ' ' << point[i].y() << ' ' << point_info[i].toStdString() << endl;

            for( int i = 1; i <= node_num; i++ )
            {
                for( int j = 1; j <= node_num; j++ )
                    out << dis_matrix[i][j] << ' ';
                out << endl;
            }

            for( int i = 1; i <= side_num; i++ )
                out << line[i].ind << ' ' << line[i].node_1 << ' ' << line[i].node_2 << ' ' << line[i].dis << endl;
            out << pic_path.toStdString() << endl;

            QMessageBox::information(this, "提示", "保存地图成功");
            out.close();        // 关闭文档
        }
        else
            QMessageBox::information(this, "提示", "保存地图失败");
    }
    ui->Btn_3_3->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
}

void Scenicmanagement::on_Btn_3_4_clicked()      // 加载背景
{
    All_flag_Clear();
    Recover();
    ui->Btn_3_4->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
    QString temp = QFileDialog::getOpenFileName(this, "选择文件", "", "图像文件(*.jpg *.jpeg *.bmg *.png *.gif)");   // 设定只支持三种图片格式
    if(temp != "")
        pic_path = temp;     // 传入图片存储路径
    update();                // 重绘(背景图片就会被画上去)
    ui->Btn_3_4->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
}

void Scenicmanagement::on_Btn_3_5_clicked()      // 清除屏幕
{
    QMessageBox::StandardButton answer;
    All_flag_Clear();
    Recover();
    ui->Btn_3_5->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_2.png);");
    answer = QMessageBox::question(this, "提示", "确认要清除屏幕吗？", QMessageBox::Yes | QMessageBox::No);
    if (answer == QMessageBox::Yes)
    {
        pic_path = "";              // 图片路径清零
        for ( int i = 1; i <= node_num; i++ )
        {
            point[i] = point[0];    // 各个点的坐标位置清零
            point_info[i] = point_info[0];          // 各个点的标签清零
            for ( int j = 1; j <= node_num; j++ )
                dis_matrix[i][j] = 0;               // 距离矩阵清零
        }
        for (int i = 1; i <= node_num; i++)
            line[i] = line[0];                      // 各条道路清零
        function_num = node_num = side_num = info_ind = 0;      // 功能号、景点数、道路数、景点标签的数组下标都清零
        ui->Message_1->clear();
    }
    ui->Btn_3_5->setStyleSheet("border-image: url(:/new/prefix1/appimages/btn_1.png);");
}

void Scenicmanagement::on_Btn_2_5_clicked()
{
    this->hide();
    emit this->backToManage();
}


void Scenicmanagement::on_scenic_info_clicked()
{
    ScenicInfoEdit* info=new ScenicInfoEdit;
    info->show();
    this->hide();
    connect(info,&ScenicInfoEdit::backToPrevious,[=](){
    this->show();
    info->hide();
    });
}

