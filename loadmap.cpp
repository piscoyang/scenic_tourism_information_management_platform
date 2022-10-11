#include "loadmap.h"
#include "ui_loadmap.h"
#include <QLabel>
LoadMap::LoadMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadMap)
{


    ui->setupUi(this);
    QString photo_path = QString(":/new/prefix1/images/map.png");
    QString imgPath = QString(photo_path);
    ui->map->setPixmap(QPixmap(imgPath));//label 加载图片imgpath
    ui->map->setScaledContents(true);    //根据label大小缩放图片

}

LoadMap::~LoadMap()
{
    delete ui;
}
