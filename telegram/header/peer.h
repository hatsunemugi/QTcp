
#ifndef PEER_H
#define PEER_H
//#include "file.h"
#include "chat.h"
#include <QtQml>
#include <QFile>
#include <QObject>
#include <QVector>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QGuiApplication>

class Peer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Peer(QObject *parent = nullptr);
    Peer(QTcpSocket*);
    Peer(QString,int);
    Peer(QString);
    ~Peer();
    void connect_from(QTcpSocket*);
    void connect_from(Peer*);
    void connect_();
    void connection();
    enum mode
    {
        Local=1,
        Server=2,
        Client=3
    };
    Q_ENUM(mode)
signals:
    void new_msg(QString);
    void connectedChanged();
    void new_peer(Peer*);
//    void confirmed();
public slots:
    QString get_say();
    QString get_name();
    QString get_ip();
    Chat* toChat();
    void file(QString);
    void send(QString);
    void reconnect();
    void new_chat();
    bool connected_();
    void disconnect_();
    void reconnect_by(Peer*);
public:
    int it;
    int mode;
    qint64 all;
    qint64 length;
    Chat* chat_;
    Peer* host;
    QString ip;
    QString port;
    QString name;
    QString latest;
    QString separator="|:|";
    bool connected;
//    bool bIsFile;
    QTcpServer* server;
    QTcpSocket* client;
    QVector<QString> history;
    QGuiApplication* gui;
};

QString _get_ip_(QString);

#endif // PEER_H
