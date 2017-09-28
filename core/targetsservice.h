#ifndef TARGETSSERVICE_H
#define TARGETSSERVICE_H

#include "targetssource.h"
#include "targetsstorage.h"
#include <QObject>

class TargetsService : public QObject
{
    Q_OBJECT
public slots:
    void TargetsReady();

public:
    TargetsService(TargetsSource &source, TargetsStorage &storage);

private:
    TargetsSource &_source;
    TargetsStorage &_storage;
};

#endif // TARGETSSERVICE_H
