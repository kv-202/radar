
#include "targetsstoragenotifiersubscriber.h"

TargetsStorageNotifierSubscriber::TargetsStorageNotifierSubscriber(const QString &service, const QString &path, const QDBusConnection &connection):
    QDBusAbstractInterface(service, path, "org.Targets.Storage.Notifier", connection, nullptr)
{
}
