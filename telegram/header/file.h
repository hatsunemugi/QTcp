
#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QStandardPaths>
#include <QGuiApplication>
class Peer;
class BackEnd;
class File : public QObject
{
    Q_OBJECT
public:
    File(BackEnd*);
    int n;
    Peer* now;
    bool bIsFile;
    bool bIsHead;
    QString name;
    int size;//接收文件的总大小
    QString ip;
    int received;//当前接收文件的大小
    QByteArray buffer;//当前接收的文件数据
    void send_file(QString);
    void send(QString);
    void send_udp(QString);
    void connection();
signals:
    void confirmed();
    void new_file(QString);
    void new_peer(QString);
private:
    int port;
    QFile* file;
    BackEnd* backend;
    QString separator="|:|";
    QTcpServer* server;
    QTcpSocket* client;
    QUdpSocket* udp;
    QGuiApplication* gui;
};

#endif // FILE_H
