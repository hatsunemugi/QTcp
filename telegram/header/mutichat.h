
#ifndef MUTICHAT_H
#define MUTICHAT_H


#include <QObject>
#include <QHash>
#include "peers.h"
class mutichat : public QObject
{
    Q_OBJECT
public:
    explicit mutichat(QObject* parent=nullptr);
    explicit mutichat(Peers*);

signals:

public slots:
    int get_n();
    QString get_c(int);
    void send_c(QString);
    Peer* get_p(int);
private:
    QVector<QString> conversation;
    QVector<Peer*> own;
    Peers* channel;
};

#endif // MUTICHAT_H
