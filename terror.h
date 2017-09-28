#ifndef TERROR_H
#define TERROR_H

#include <QString>

class TError {
public:
    TError(const QString &msg);

    QString Msg() { return _msg; }

    static void SetCurrent(TError *value);

    static TError *GetCurrent() { return _current; }

private:
    QString _msg;

private:
    static TError *_current;
};

#endif // TERROR_H
