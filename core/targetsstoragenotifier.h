#ifndef CLIENTTARGETSSTORAGE_H
#define CLIENTTARGETSSTORAGE_H

#include <QObject>
#include <QStringList>

class TargetsStorageNotifier : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.Targets.Storage.Notifier")
signals:
    void TargetsAdded(QStringList targets);
};

#endif // CLIENTTARGETSSTORAGE_H
