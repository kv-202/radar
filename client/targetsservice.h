#ifndef TARGETSSERVICE_H
#define TARGETSSERVICE_H

#include "clienttargetsstorage.h"
#include <QObject>

class TargetsService : public QObject
{
    Q_OBJECT
private slots:
    void TargetsAdded(QStringList targets);

public:
    TargetsService(ClientTargetsStorage &storage);

private:
    ClientTargetsStorage &_storage;
};

#endif // TARGETSSERVICE_H
