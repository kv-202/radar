
#include "targetsservice.h"
#include "../config.h"
#include "../terror.h"
#include <QDateTime>
#include <iostream>

void TargetsService::TargetsAdded(QStringList targets)
{
    QTime tSet = QTime::currentTime();

    QVector<Target> values;
    if (!_storage.GetLastTargets(targets, values))
    {
        std::cout<<TError::GetCurrent()->Msg().toStdString()<<std::endl;
        return;
    }

    int setMsec = tSet.msecsTo(QTime::currentTime());
    std::cout<<QDateTime::currentDateTime().toString().toStdString()<<": get - "<<setMsec<<std::endl;

    foreach (const Target &value, values)
    {
        std::cout<<value._name.toStdString()<<std::endl;

        foreach (const Event &event, value._events)
        {
            std::cout<<"    x - "<<event._point._x;
            std::cout<<" y - "<<event._point._y;
            std::cout<<" z - "<<event._point._z;
            std::cout<<" state - "<<event._state;
            std::cout<<" time - "<<event._time.toString(_DT_FORMAT).toStdString();
            std::cout;
        }

        std::cout<<std::endl;
    }

    std::cout<<std::endl;
}

TargetsService::TargetsService(ClientTargetsStorage &storage):
    _storage(storage)
{
    QObject::connect(&_storage, &ClientTargetsStorage::TargetsAdded, this, &TargetsService::TargetsAdded);
}
