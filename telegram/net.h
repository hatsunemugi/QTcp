#ifndef NET_H
#define NET_H

#include <QObject>

class Net : public QObject
{
    Q_OBJECT
public:
    explicit Net(QObject *parent = nullptr);

signals:

};

#endif // NET_H
