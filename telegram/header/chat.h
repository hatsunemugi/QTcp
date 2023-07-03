
#ifndef CHAT_H
#define CHAT_H


#include <QObject>
#include <QVector>
#include <QtQml>
class Peer;
struct Message{
    QString body;
    bool bIsFile;
    double process;
    Peer* from;
    Message(Peer* peer,QString text):bIsFile(false),process(0),from(peer),body(text){}
};
class Chat : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Chat(QObject* parent=nullptr);
    Chat(Peer*,Peer*);
    Peer* remote;
    double bProcess;
    enum mode
    {
        IsFile=1,
        IsChat=2
    };
    Q_ENUM(mode)
signals:
    void new_msg(QString);
    void new_file(QString);
    void process(double);
public slots:
    int get_n();
    QString get_c(int);
    QString get_name(int);
    QString get_ip(int);
    Peer* get_remote(){return remote;}
    Peer* get_Local(){return local;}
    bool bIsFile(int);
    Peer* get_p(int);
    void send_msg(QString);
    void recv_msg(QString);
    void send_file(QString);
    void recv_file(QString);
    void set_file_process(int,double);
    double get_file_process_by(int);

private:
    Peer* local;
    QVector<Message*> Messages;
    QVector<QString> conversation;
    QVector<int> from;
    QVector<int> is_file;
    QVector<double> file_process;
    Message* latest;
    Message* latest_file;
};

#endif // CHAT_H
