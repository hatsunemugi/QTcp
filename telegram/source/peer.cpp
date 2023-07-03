#include "peer.h"
#include <QDir>
#include <QFile>
#include <QThread>
#include <QWidget>
//#include "runnable.h"
#include <QThreadPool>
#include <QFileDialog>
#include <QHostAddress>
#include <QNetworkInterface>
//#include "fmt/core.h"
//using namespace fmt;
Peer::Peer(QObject *parent)
    : QObject{parent}
{
    mode=mode::Local;
    ip=_get_ip_("192.168");
    port="10086";
    name=QDir::home().dirName();
    server=new QTcpServer(this);
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    server->listen(QHostAddress::Any,port.toUShort());
    connect(server,&QTcpServer::newConnection,this,[&](){
        client=server->nextPendingConnection();
        Peer* peer_=new Peer(client);
        peer_->gui=gui;
        qDebug()<<"connected by"<<peer_->ip;
        peer_->host=this;
        peer_->new_chat();
        peer_->mode=mode::Server;
        peer_->connected=true;
        emit new_peer(peer_);
        send("config"+separator+QDir::home().dirName());
    });
}

Peer::Peer(QTcpSocket *client_)
{
    mode=mode::Client;
    QString s=client_->peerAddress().toString();
    ip=s.split(":").back();
    port=client_->peerPort();
    name=client_->peerName();
    client=client_;
    connection();
    connect(client,&QTcpSocket::disconnected,this,[&](){
        qDebug()<<"disconnected from"<<ip;
        connected=false;
        client->close();
        client->deleteLater();
    });
}

Peer::Peer(QString ip_)
{
    mode=mode::Client;
    qDebug()<<ip_;
    ip=ip_;
    port="10086";
//    bInited=false;
//    bIsFile=false;
    client = new QTcpSocket(this);
    client->setProxy(QNetworkProxy::NoProxy);
    client->connectToHost(QHostAddress(ip),port.toUShort());
    connection();
    connect(client,&QTcpSocket::connected,this,[&](){
        connected=true;
        send("config"+separator+QDir::home().dirName());
    });
    connect(client,&QTcpSocket::disconnected,this,[&](){
        qDebug()<<"disconnected from"<<ip;
        connected=false;
        client->close();
    });
}

Peer::~Peer()
{
//    disconnect();
}

void Peer::connect_from(QTcpSocket *client_)
{
    client=client_;
    emit connectedChanged();
    //    connection();
}

void Peer::connect_from(Peer *peer_)
{
    client=peer_->client;
    chat_->remote=peer_->chat_->remote;
    emit connectedChanged();
}

void Peer::connect_()
{
    chat_=new Chat(host,this);
    client->connectToHost(QHostAddress(ip),port.toUShort());
    connection();
}

void Peer::reconnect()
{
    switch (mode) {
    case mode::Client:
        client->connectToHost(QHostAddress(ip),port.toUShort());
        break;
    case mode::Server:
//        client->connectToHost(QHostAddress(ip),port.toUShort());
        break;
    default:
        break;
    }

}

void Peer::new_chat()
{
    chat_=new Chat(host,this);
}

bool Peer::connected_()
{
    return connected;
}

void Peer::disconnect_()
{
    connected=false;
    emit connectedChanged();
    client->disconnectFromHost();
    client->close();
}

void Peer::reconnect_by(Peer *p)
{
    client=p->client;
    connected=true;
    connection();
    connect(client,&QTcpSocket::connected,this,[&](){
        connected=true;
        //        emit connectedChanged();
    });
    connect(client,&QTcpSocket::disconnected,this,[&](){
        qDebug()<<"disconnected from"<<ip;
        //        emit connectedChanged();
        connected=false;
        client->close();
        client->deleteLater();
    });
    p->deleteLater();
}

QString Peer::get_say()
{
    return latest;
}

QString Peer::get_name()
{
    return name;
}

QString Peer::get_ip()
{
    return ip;
}

Chat *Peer::toChat()
{
    qDebug()<<"to chat";
    return chat_;
}

void Peer::file(QString filename)
{
    qDebug()<<"peer view file"<<filename<<ip;
    latest=filename;
    history.push_back(filename);
}

void Peer::send(QString text)
{
    client->write(text.toUtf8());
    qDebug()<<"send"<<text;
}

void Peer::connection()
{
    connect(client,&QTcpSocket::readyRead,this,[&](){
        QByteArray data=client->readAll();
        QString s(data);
        qDebug()<<s<<"from"<<ip;

        if(s.startsWith("config"+separator))
        {
            QStringList sl=s.split(separator);
            name=sl[1];
        }
        else
        {
            latest=s;
            history.push_back(s);
            emit new_msg(s);
            chat_->recv_msg(s);
            qDebug()<<ip<<":"<<s;
        }
    });
}


QString _get_ip_(QString head)
{
    QString ip_address;
    QList<QHostAddress> ip_addresses_list = QNetworkInterface::allAddresses(); // 获取所有ip
    for (int i = 0; i < ip_addresses_list.size(); ++i)
    {
        QHostAddress ip = ip_addresses_list.at(i);
        if ((ip.protocol() == QAbstractSocket::IPv4Protocol))// 筛选出ipv4
        {
            qDebug() << "---- ipv4 :" << ip.toString();
            if (ip.toString().startsWith(head))// 过滤掉网卡，路由器等的ip
            {
                ip_address = ip_address.isEmpty()?ip.toString():ip_address;
//                break;
            }
        }
    }
    if (ip_address.isEmpty())
        ip_address = QHostAddress(QHostAddress::LocalHost).toString();
    qDebug() << "----ip set:" << ip_address;
    return ip_address;
}
