
#ifndef RUNNABLE_H
#define RUNNABLE_H

#include <QFile>
#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QElapsedTimer>
class runnable : public QObject,public QRunnable
{
    Q_OBJECT
public:
    int i;
    bool bStop;
    QFile* file;
    QString separator="|:|";
    QString filepath;
    qint64 length; //当前已经发送的大小
    qint64 all; //总文件大小
    QString ip;
    QString head;
    int port;
    QTcpSocket* socket;
    QElapsedTimer* timer;
    explicit runnable(QString,QString,int port_=52000);
protected:
    void run() override;
signals:
    void done();
    void confirmed();
//    void send(double);
private:
};

#endif // RUNNABLE_H
