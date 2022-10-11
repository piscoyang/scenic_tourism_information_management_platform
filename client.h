#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QDateTime>
#include <QMessageBox>
#include <QSound>
#include <QIcon>
namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    void getTouristname(QString name);
    void getTouristID(QString id);
    QString name;
    QString id;
    ~Client();
    QSound *messageSound;
    QSound *connectSound;
signals:
    void backToSelect();
private slots:
    void on_connectBtn_clicked();

    void on_sendBtn_clicked();
    void readMessage();         //鎺ユ敹淇℃伅

    void disconnectSlot();          //鏂紑杩炴帴妲藉嚱鏁

    void on_back_btn_clicked();

private:
    Ui::Client *ui;
    QTcpSocket *socket;
    bool connectState;  //瀹㈡埛绔繛鎺ョ姸鎬
};

#endif // CLIENT_H
