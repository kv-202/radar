#ifndef TARGETSSTORAGE_H
#define TARGETSSTORAGE_H

#include "../target.h"
#include "targetsstoragenotifier.h"
#include <QVector>
#include <QString>
#include <QtSql>

class TargetsStorage
{
public:
    TargetsStorage();

    bool Open(const QString &file);

    bool AddTargets(const QVector<Target> &values);

private:
    QSqlDatabase _database;
    TargetsStorageNotifier _notifier;
};

#endif // TARGETSSTORAGE_H
