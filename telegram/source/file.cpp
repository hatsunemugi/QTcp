#include "runnable.h"
#include "file.h"
#include "backend.h"
#include <QThreadPool>


File::File(BackEnd *parent)
{
    port=52000;
    backend=parent;
    setParent(parent);
    gui=backend->gui;
    udp=new QUdpSocket(this);
    server=new QTcpServer(this);
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    server->listen(QHostAddress::Any,port);
    udp->bind(QHostAddress::Any,port+1);
    connect(server,&QTcpServer::newConnection,this,[&](){
        client=server->nextPendingConnection();
        QString s=client->peerAddress().toString().split(":").back();
//        qDebug()<<"file from"<<s;
        ip=s;
        emit new_peer(s);
        connect(client,&QTcpSocket::disconnected,this,[&](){qDebug()<<"file disconnection from"<<ip;/*now->bIsFile=false;*/});
        connect(client,&QTcpSocket::readyRead,this,[&](){
            QByteArray data=client->readAll();
            QString s(data);
//            qDebug()<<s;
            if(s.startsWith("file"+separator)&&s.size()>=8)
            {
                qDebug()<<"file in";
                QStringList sl=s.split(separator);
                for(int i=0;i<sl.size();i++)
                    qDebug()<<sl[i];
                if(sl.size()==3)
                {
                    n=0;
                    buffer.clear();
                    received=0;
                    name=sl[1];
                    size=sl[2].toInt();
                    send("ok");
                    qDebug()<<"name"<<name;
//                    qDebug()<<backend->now->name;
                    now->toChat()->recv_file(name);
                }
            }
            else
            {
                //根据文件名和文件大小接收和保存文件
                n++;
                buffer.append(data);
                received += data.size();//每接收一次文件，当前文件大小+1
                double process=(double)received/size;
                qDebug()<<process;
                emit now->toChat()->process(process);
//                qDebug()<<received<<"/"<<size;
                if(n%4==0||process==1||n<=7)//to slow down update frequency
                {
                    send_udp(QString::number(process,'f',4));
//                    now->send(QString::number(process,'f',4));
                }

                //当接收文件大小等于总文件大小，即文件数据接收完毕
                if (received>=size)
                {
                    //保存文件
                    #ifdef Android_Platform
                    QString filepath="/sdcard/Download/qtelegram/";
                    #else
                    QString filepath=QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) +"/";
                    #endif
                    QDir dir;
                    if(dir.mkpath(filepath)){
                        filepath+=name;
                        qDebug()<<filepath;
                        file=new QFile(filepath);
                        if (!file->open(QIODevice::WriteOnly))
                        {
                            qDebug()<<"fail to open";
//                            now->bIsFile = false;
                            file->deleteLater();
                            //读取文件失败
                            return;
                        }
                        file->write(buffer);
                        file->close();
//                        now->bIsFile = false;
                        file->deleteLater();
                    }
                }
            }
        });
    });
    connect(udp,&QUdpSocket::readyRead,this,[&](){
        QString s;
        while (udp->hasPendingDatagrams()) {
            QNetworkDatagram datagram = udp->receiveDatagram();
            s = datagram.data();
        }
        qDebug()<<s;
        double value = s.toDouble();
        if(now!=nullptr)
            now->chat_->process(value);
    });
}


void File::send_file(QString filepath)
{
//    QThreadPool::globalInstance()->start(new runnable(filepath,ip,port));
    qDebug()<<"start sending file";
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
    QString head=QString("file|:|%1|:|%2").arg(filename).arg(file->size());
    client=new QTcpSocket(this);
    qDebug()<<"file try connecting to"<<ip<<port;
    client->connectToHost(QHostAddress(ip),port);
    connect(client,&QTcpSocket::disconnected,this,[=](){/*now->bIsFile=false;*/});
    connect(client,&QTcpSocket::connected,this,[=](){qDebug()<<head;emit new_peer(ip); now->toChat()->send_file(filename);client->write(head.toUtf8());});
    connect(client,&QTcpSocket::readyRead,this,[&](){
        QByteArray data=client->readAll();
        QString s(data);
        qDebug()<<s<<"from"<<ip<<port;
        if(s=="ok")
        {
            emit confirmed();
        }
    });
    connect(this,&File::confirmed,this,[&](){
        qDebug()<<"file out";
//        now->bIsFile=true;
        qint64 length = 0;//当前已经发送的大小
        qint64 all = file->size();//总文件大小
        while (length < all)//当发送文件等于文件大小时，发送完毕，循环结束
        {
            gui->processEvents();
            char data[1024];
            qint64 bs = file->read(data, 1024);//读文件放入data数组中，返回读取到的大小
            client->write(data, bs);//把读取到的data数据发送给服务器
            length += bs;//实时获取当前发送的文件大小
        }
//        now->bIsFile=false;
    });
}

void File::send(QString text)
{
    client->write(text.toUtf8());
    qDebug()<<text;
}

void File::send_udp(QString text)
{
    if(now==nullptr)
        return;
    qDebug()<<"udp sending"<<text<<"to ip:"<<now->ip<<port+1;
    QUdpSocket socket;
    socket.writeDatagram(QNetworkDatagram(text.toUtf8(),QHostAddress(now->ip),port+1));
}

void File::connection()
{

}

