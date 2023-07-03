#ifndef BACKEND_H
#define BACKEND_H
#include <QtQml>
#include <QTimer>
#include <QObject>
#include <QVector>
#include <QGuiApplication>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "mutichat.h"
#include "chat.h"
#include "file.h"
class BackEnd : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    Peer* now;
    QGuiApplication* gui;
    explicit BackEnd(QObject *parent = nullptr);
    void peer_in(Peer*);
    static void log_(QtMsgType type, const QMessageLogContext &context, const QString &msg);
public slots:
    void scan();
    void set_channel(QString,QString);
    QString local_ip();
    int get_log_n();
    QString get_log_by(int);
    int get_peer_n();
    Peer* get_peer_by(int);
    void connect_to(QString);
//    void connect_to(Peer*);
    QStringList get_prefix();
    QStringList paste_ip();
    void copy_ip(QString);
    void send_file(QString);
    void set_ip(QString);
    void setPeer(Peer*);
    void exec(QString);
    int exists(Peer*);
    int exists(QString);
signals:
    void done();
    void new_peer(Peer*);
    void new_msg_from(QString,Peer*);
    void new_log(QString);
//    void process(double);
private:
    QTimer* timer;
    Peer* local;
    QRegularExpression rule_ip;
    QRegularExpressionMatch match;
    QVector<mutichat*> mutichat_;
    QVector<Chat*> chat_;
    QVector<Peer*> peers;
    QString ip_prefix;
    QString ip_min;
    QString ip_max;
    QString port;
    File* file_tcp;
    int lefted;
    int from;
    int to;
};

#endif // BACKEND_H
