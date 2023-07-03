
#include "runnable.h"
#include <QHostAddress>
runnable::runnable(QString filepath_, QString ip_, int port_)
//    : QObject{filepath}
{
    setAutoDelete(false);
    filepath=filepath_;
    ip=ip_;
    port=port_;
    length=0;
    qDebug()<<"file try connecting to"<<ip<<port;
    //    qDebug()<<head;
    //    socket->write(head.toUtf8());
}
void runnable::run()
{
    timer=new QElapsedTimer();
    timer->start();
    socket=new QTcpSocket();
    connect(socket,&QTcpSocket::connected,this,[&](){qDebug()<<"send head";socket->write(head.toUtf8());});
    connect(socket,&QTcpSocket::readyRead,this,[&](){
        QByteArray data=socket->readAll();
        QString s(data);
        qDebug()<<s<<"from"<<ip<<port;
        if(s=="ok")
        {
            emit confirmed();
        }
//        else if(s=="file connected")
//        {
//            qDebug()<<head;
//            socket->write(head.toUtf8());
//        }
    });
    connect(this,&runnable::confirmed,this,[&](){
        qDebug()<<"file out";
        length = 0;//当前已经发送的大小
        all = file->size();//总文件大小
        while (length < all)//当发送文件等于文件大小时，发送完毕，循环结束
        {
            char data[1024];
            qint64 bs = file->read(data, 1024);//读文件放入打他数组中，返回读取到的大小
            socket->write(data, bs);//把读取到的data数据发送给服务器
            length += bs;//实时获取当前发送的文件大小
        }
        emit done();
    });
    connect(this,&runnable::done,this,[&](){qDebug()<<timer->elapsed();deleteLater();});
    filepath=filepath.split("///").back();
    qDebug()<<filepath;
    file=new QFile(filepath);
    if (!file->open(QIODevice::ReadWrite))
    {
        qDebug()<<"fail to open";
        //读取文件失败
        return;
    }
    QString filename=file->fileName().split("/").back();
    head=QString("file%1%2%3%4").arg(separator).arg(filename).arg(separator).arg(file->size());
    socket->connectToHost(QHostAddress(ip),port);
    qDebug()<<head;
    try {
        socket->write(head.toUtf8());
    } catch (...) {
        qDebug()<<"not connected yet";
    }
}
