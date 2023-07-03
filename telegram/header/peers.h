
#ifndef PEERS_H
#define PEERS_H


#include <QObject>
#include <QVector>
#include "peer.h"

class Peers : public QObject
{
    Q_OBJECT
public:
    explicit Peers(QObject *parent = nullptr);
    Peer* byname(QString);
    Peer* byip(QString);
signals:

private:
    Peer* local;
    QVector<Peer*> remotes;

};

#endif // PEERS_H
