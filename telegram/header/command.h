
#ifndef COMMAND_H
#define COMMAND_H

#include <QRegularExpression>
#include <QTextStream>
#include <QObject>
#include <QVector>
#include <QQueue>
#include <QStack>
#include <QDebug>
struct Word{
    QString type;
    QString body;
    Word* prev;
    Word* next;
    friend QDebug operator <<(QDebug dbg, const Word& p)
    {
        dbg<<p.type<<p.body;
        return dbg;
    }
    friend bool operator ==(const Word& p, const QString& s)
    {
        return p.body==s;
    }
    friend bool operator ==(const QVector<Word>& cmd, const QString& s)
    {
        QStringList sl=s.split(" ");
        if(sl.back()=="")
            sl.pop_back();
        //        qDebug()<<cmd.size()<<s<<sl.size();
        if(cmd.size()==sl.size())
        {
            //        qDebug()<<"length match";
            if(sl[0]=="set")
                return cmd[0]==sl[0]&&cmd[1]==sl[1];
            else if(sl[0]=="unset")
                return cmd[0]==sl[0]&&cmd[1]==sl[1];
            else
                return cmd[0]==sl[0];
        }
        return false;
    }
    friend QDebug operator <<(QDebug dbg, const QVector<Word>& a)
    {
        foreach (const Word& word, a) {
            qDebug()<<word;
        }
        return dbg;
    }
    friend void print(const QVector<Word>& a)
    {
        foreach (const Word& word, a) {
            qDebug()<<word;
        }
    }
};

class Command : public QObject
{
    Q_OBJECT
public:
    enum State{
        START = 1,
        IDENTIFIER_OR_KEYWORD = 2,
        IDENTIFIER=3,
        OPERATOR=4,
        SEPARATOR=5,
        NUMBER=6,
        DONE=7
    };
public:
    explicit Command(QObject *parent = nullptr);
    static QVector<Word> analyse(QString);
    static void append(State,QString);
    static bool isLetter(QString s){return ("a"<=s&&s<="z")||("A"<=s&&s<="Z")||(QChar(0x4e00)<=s[0]&&s[0]<=QChar(0x9fa5));}
    static bool isDigit(QString s){return "0"<=s&&s<="9";}
    static bool isOperator(QString);
    static bool isSeparator(QString);
    static bool isKeyword(QString);
    static bool isEmpty(QString);
signals:
    void done();
private:
    static QRegularExpression space;
    static QVector<QString> operators;
    static QVector<QString> separators;
    static QVector<QString> keywords;
    static QVector<Word> result;
    static State state;
};

#endif // COMMAND_H
