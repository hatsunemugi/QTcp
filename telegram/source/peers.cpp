
#include "peers.h"
Peers::Peers(QObject *parent)
    : QObject{parent}
{
    local = new Peer;
    local->ip=_get_ip_("192.168");
    local->name="local";
    local->port="55555";
}

