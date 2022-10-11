#ifndef LOADMAP_H
#define LOADMAP_H

#include <QDialog>

namespace Ui {
class LoadMap;
}

class LoadMap : public QDialog
{
    Q_OBJECT

public:
    explicit LoadMap(QWidget *parent = nullptr);
    ~LoadMap();

private:
    Ui::LoadMap *ui;
};

#endif // LOADMAP_H
