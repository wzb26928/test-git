#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QTcpSocket>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
public:
    QTcpSocket *mysck;
    QPixmap *pic;
    int flags;
    int fl;



private slots:
    void on_pushButton_connect_clicked();
    void myconnect_success();
    void on_pushButton_recv_pic_clicked();
    void recv_pic_show();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
