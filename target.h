#ifndef TARGET_H
#define TARGET_H

#include <QString>
#include <QDateTime>
#include <QVector>

struct Point
{
    int _x;
    int _y;
    int _z;
};

struct Event
{
    QDateTime _time;
    Point _point;

    enum State
    {
        Found = 0x1,
        Leading = 0x2,
        Lost = 0x3,
    };

    State _state;
};

struct Target
{
    QString _name;
    QVector<Event> _events;
};


#endif // TARGET_H
