#ifndef CLIENTTARGETSSTORAGE_H
#define CLIENTTARGETSSTORAGE_H

#include "targetsstoragenotifiersubscriber.h"
#include "../target.h"
#include <QObject>
#include <QStringList>
#include <QtSql>

class ClientTargetsStorage : public QObject
{
    Q_OBJECT
signals:
    void TargetsAdded(QStringList targets);

private slots:
    void TargetsAddedEmitter(QStringList targets);

public:
    ClientTargetsStorage();

    bool Open(const QString &file);

    bool GetTargets(const QStringList &targets, const QDateTime &from, const QDateTime &to, QVector<Target>& values);

    bool GetLastTargets(const QStringList &targets, QVector<Target>& values);

private:
    QSqlDatabase _database;
    TargetsStorageNotifierSubscriber _notifier;
};

#endif // CLIENTTARGETSSTORAGE_H
