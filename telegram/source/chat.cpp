#include "peer.h"
#include "chat.h"
Chat::Chat(QObject *parent):QObject{parent}
{

}

Chat::Chat(Peer* local_,Peer* remote_)

{
    local=local_;
    remote=remote_;
    connect(this,&Chat::process,this,[&](double d_process){latest_file->process=d_process;});
    qDebug()<<"new Chat";
}

int Chat::get_n()
{
    qDebug()<<Messages.size();
    return Messages.size();
}

QString Chat::get_c(int i)
{
    try {
        return Messages[i]->body;
    } catch (...) {
        return "error";
    }
}

QString Chat::get_name(int i)
{
    return get_p(i)->name;
}

QString Chat::get_ip(int i)
{
    return get_p(i)->ip;
}

bool Chat::bIsFile(int i)
{
    return Messages[i]->bIsFile;
}

Peer *Chat::get_p(int i)
{
    try {
        return Messages[i]->from;
    } catch (...) {
        return local;
    }
}

void Chat::send_msg(QString text)
{
    Messages.push_back(new Message(local,text));
    remote->send(text);
    emit new_msg(text);
}

void Chat::recv_msg(QString text)
{
    qDebug()<<"chat view message";
    latest=new Message(remote,text);
    Messages.push_back(latest);
    emit new_msg(text);
}

void Chat::send_file(QString filepath)
{
    qDebug()<<"chat view send file";
    QString filename=filepath.split("/").back();
    local->file(filename);
    latest_file = new Message(local,filename);
    latest_file->bIsFile=true;
    latest=latest_file;
    Messages.push_back(latest_file);
    emit new_msg(filename);
}


void Chat::recv_file(QString text)
{
    qDebug()<<"chat view receive file";
    remote->file(text);
    latest_file = new Message(remote,text);
    latest_file->bIsFile=true;
    latest=latest_file;
    Messages.push_back(latest_file);
    emit new_msg(text);
}

void Chat::set_file_process(int index, double value)
{
    Messages[index]->process=value;
}

double Chat::get_file_process_by(int index)
{
    return Messages[index]->process;
}


