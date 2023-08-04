#include "BackEnd.h"
#include <QClipboard>
#include "command.h"
//#include <windows.h>
QtMessageHandler default_log = nullptr;
QVector<QString> logs;
static BackEnd* backend_;
QString _ip_;
QString _port_;
BackEnd::BackEnd(QObject *parent)
    : QObject{parent}
{
    backend_=this;
    rule_ip.setPattern("(\\d+).(\\d+).(\\d+).(\\d+)");
    gui=(QGuiApplication*)parent;
    default_log=qInstallMessageHandler(log_);
    local=new Peer(this);
    local->gui=gui;
    _port_ = "10086";
    file_tcp=new File(this);
//    timer=new QTimer;

//    connect(file_tcp,&File::new_file,this,[&](QString filename){now->send_file(filename);});
    connect(file_tcp,&File::new_peer,this,[&](QString ip){
        try{
            int n=exists(ip);
            if(n>-1)
            {
                now=peers[n];
                file_tcp->now=now;
                qDebug()<<"file from peer"<<now->name<<"at"<<now->ip;
            }
            else
                qDebug()<<"peer now "<<n<<"is invalid";
        } catch (...) {
            qDebug()<<"no peer at"<<ip;
        }
    });
//    connect(timer, &QTimer::timeout, this, [&](){});
    connect(local,&Peer::new_peer,this,&BackEnd::peer_in);
//    timer->start(10000);
    set_channel("192.168.31.0","192.168.31.255");
}

void BackEnd::peer_in(Peer *p)
{
    int n=exists(p);
    if(n<0){
        peers.push_back(p);
        qDebug()<<"new peer";
        emit new_peer(p);
    }
    else{
        peers[n]->reconnect_by(p);
    }

}

void BackEnd::log_(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString text;
    text.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " ");
    switch(type)
    {
    case QtDebugMsg:
        text.append("Debug:");
        break;

    case QtWarningMsg:
        text.append("Warning:");
        break;

    case QtCriticalMsg:
        text.append("Critical:");
        break;

    case QtFatalMsg:
        text.append("Fatal:");

    default:
        break;
    }
    text.append(msg);
    logs.push_back(msg);
    emit backend_->new_log(msg);
    default_log(type,context,msg);
}


void BackEnd::scan()
{
    switch (lefted) {
    case 1:
        for (int i = from; i < to; ++i) {
            QString ip=ip_prefix+QString::number(i);
//            qDebug()<<"try"<<ip<<local->port;
            local->client->connectToHost(QHostAddress(ip),local->port.toUShort());
            connect(local->client,&QTcpSocket::connected,this,[=](){qDebug()<<"connected to"<<ip<<local->port;});
        }
        break;
    case 2:
        for (int i = from; i < to; ++i) {
            for (int j = 0; j < 255; ++j) {
                QString ip=ip_prefix+QString::number(i)+"."+QString::number(j);
//                qDebug()<<"try"<<ip<<local->port;
                local->client->connectToHost(QHostAddress(ip),local->port.toUShort());
                connect(local->client,&QTcpSocket::connected,this,[=](){qDebug()<<"connected to"<<ip<<local->port;});
            }
        }
        break;
    default:
        break;
    }
}

void BackEnd::set_channel(QString ip_min, QString ip_max)
{
    this->ip_min=ip_min;
    this->ip_max=ip_max;
    match=rule_ip.match(ip_min);
    QVector<QString> min;
    for (int i = 0; !match.captured(i).isEmpty(); i++) {
        min.push_back(match.captured(i));
        qDebug()<<match.captured(i);
    }
    match=rule_ip.match(ip_max);
    QVector<QString> max;
    for (int i = 0; !match.captured(i).isEmpty(); i++) {
        max.push_back(match.captured(i));
        qDebug()<<match.captured(i);
    }
    ip_prefix="";
    int n=0;
    for (int i = 0; i < min.size(); i++) {
        if(min[i]==max[i]&&min[i].length()<4)
        {
            ip_prefix+=min[i];
            if(n<3)
                ip_prefix+=".";
            n++;
            from=min[i+1].toInt();
            to=max[i+1].toInt();
        }
    }
    lefted=4-n;
    qDebug()<<"ip prefix set"<<ip_prefix<<lefted;
}

QString BackEnd::local_ip()
{
    return local->ip;
}

int BackEnd::get_log_n()
{
    return logs.size();
}

QString BackEnd::get_log_by(int i)
{
    return logs[i];
}

int BackEnd::get_peer_n()
{
    return peers.size();
}

Peer *BackEnd::get_peer_by(int i)
{
    return peers[i];
}

void BackEnd::connect_to(QString ip)
{
    if(ip.isEmpty())
        return ;
    int n=exists(ip);
    if(n<0)
    {
        Peer* remote=new Peer(ip,_port_.toInt());
        remote->gui=gui;
        remote->host=local;
        remote->new_chat();
        peers.push_back(remote);
        qDebug()<<"new peer";
        emit new_peer(remote);
    }
}

QStringList BackEnd::get_prefix()
{
    QStringList prefix=ip_prefix.split('.');
    return prefix;
}

QStringList BackEnd::paste_ip()
{
    QClipboard *clipboard=QGuiApplication::clipboard();
    QString ip_=clipboard->text();
    QStringList ip=ip_.split('.');
    qDebug()<<"paste ip"<<ip;
    return ip;
}

void BackEnd::copy_ip(QString ip)
{
    QClipboard *clipboard=QGuiApplication::clipboard();
    clipboard->setText(ip);
    qDebug()<<"copy"<<ip;
}

void BackEnd::send_file(QString filepath)
{
//    file_tcp->ip=ip;
    file_tcp->now=this->now;
    file_tcp->send_file(filepath);
}

void BackEnd::set_ip(QString ip)
{
    file_tcp->ip=ip;
}

void BackEnd::setPeer(Peer *p)
{
    now=p;
}

void BackEnd::exec(QString content)
{
    if(content.isEmpty())
        return ;
    if(Command::isEmpty(content))
        return ;
    QVector<Word> cmd=Command::analyse(content);
    print(cmd);
    if(cmd=="write value")
    {
        QUdpSocket socket;
        socket.writeDatagram(QNetworkDatagram(cmd[1].body.toUtf8(),QHostAddress(_ip_),_port_.toInt()));
    }
    else if(cmd=="set ip value")
    {
        _ip_=cmd[2].body;
        qDebug()<<"ip is set to"<<_ip_;
    }
    else if(cmd=="set port value")
    {
        _port_=cmd[2].body;
        qDebug()<<"port is set to"<<_port_;
    }
}

int BackEnd::exists(Peer *p)
{
    if(peers.size()==0)
        return -2;
    int n=-1;
    for (int i = 0; i < peers.size(); ++i) {
        if(peers[i]->ip==p->ip)
        {
            n=i;
            break;
        }
    }
    return n;
}

int BackEnd::exists(QString ip)
{
    if(peers.size()==0)
        return -2;
    int n=-1;
    for (int i = 0; i < peers.size(); ++i) {
        if(peers[i]->ip==ip)
        {
            n=i;
            break;
        }
    }
    return n;
}






