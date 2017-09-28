
#include "targetsservice.h"
#include "../terror.h"
#include <QTime>
#include <iostream>

void TargetsService::TargetsReady()
{
    QVector<Target> values;
    if (!_source.GetTargets(values))
    {
        std::cout<<TError::GetCurrent()->Msg().toStdString()<<std::endl;
        return;
    }

    QTime tSet = QTime::currentTime();

    if (!_storage.AddTargets(values))       
        std::cout<<TError::GetCurrent()->Msg().toStdString()<<std::endl;

    int setMsec = tSet.msecsTo(QTime::currentTime());
    std::cout<<QDateTime::currentDateTime().toString().toStdString()<<": set - "<<setMsec<<std::endl;
}

TargetsService::TargetsService(TargetsSource &source, TargetsStorage &storage):
    _source(source),
    _storage(storage)
{
    QObject::connect(&_source, &TargetsSource::TargetsReady, this, &TargetsService::TargetsReady);
}
