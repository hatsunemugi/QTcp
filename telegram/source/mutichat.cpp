
#include "mutichat.h"

mutichat::mutichat(QObject *parent):
    QObject{parent}
{

}

mutichat::mutichat(Peers *channel)
{
    this->channel=channel;
}

int mutichat::get_n()
{
    return conversation.size();
}

QString mutichat::get_c(int)
{

}

void mutichat::send_c(QString)
{

}

Peer *mutichat::get_p(int)
{

}

