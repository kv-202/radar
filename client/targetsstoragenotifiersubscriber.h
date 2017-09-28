#ifndef TARGETSSTORAGENOTIFIERSUBSCRIBER_H
#define TARGETSSTORAGENOTIFIERSUBSCRIBER_H

#include <QStringList>
#include <QDBusAbstractInterface>

class TargetsStorageNotifierSubscriber : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    TargetsStorageNotifierSubscriber(const QString &service, const QString &path, const QDBusConnection &connection);

signals:
    void TargetsAdded(QStringList targets);
};

#endif // TARGETSSTORAGENOTIFIERSUBSCRIBER_H
