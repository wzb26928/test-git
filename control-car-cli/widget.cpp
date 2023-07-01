#include "widget.h"
#include "ui_widget.h"
#include <QTcpSocket>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineEdit_ipaddr->setText("192.168.0.82");
    ui->lineEdit_port->setText("8888");
    this->mysck = new QTcpSocket();
    this->flags = 0;
    this->setWindowTitle("图片云盘客户端V1");

    connect(this->mysck, SIGNAL(connected()), this, SLOT(myconnect_success()));
    connect(this->mysck, SIGNAL(readyRead()), this, SLOT(recv_pic_show()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_connect_clicked()
{
    this->mysck->connectToHost(ui->lineEdit_ipaddr->text(), ui->lineEdit_port->text().toShort());
}
void Widget::myconnect_success()
{
     ui->label_pic->setText("连接服务器成功");
}
void Widget::on_pushButton_recv_pic_clicked()
{
    ui->label_pic->clear();
    QString cmd = "get 1.jpg\n";
    this->mysck->write(cmd.toLatin1());
}
void Widget::recv_pic_show()
{
    if(this->flags == 0)
    {
        char flen[20] = {0};
        this->mysck->read(flen,20);
        qDebug() << flen;
        this->flags = 1;
        fl = atoi(flen+4);
    }
    else if(this->flags == 1)
    {
        qDebug() << fl;
        char *mypb = NULL;
        mypb = (char *)malloc(fl);
        memset(mypb,0,fl);
        if(this->mysck->bytesAvailable() >= fl)
        {
            this->mysck->read(mypb, fl);
            this->pic = new QPixmap();
            this->pic->loadFromData((unsigned char *)mypb,fl,"jpeg");
            ui->label_pic->setPixmap(*pic);
            this->flags = 0;
        }
    }
}

