
#include "targetsstorage.h"
#include "../terror.h"
#include "../config.h"
#include <QSqlQuery>
#include <QDBusConnection>

TargetsStorage::TargetsStorage():
    _database(QSqlDatabase::addDatabase("QMYSQL", "core_app"))
{
}

bool TargetsStorage::Open(const QString &file)
{
    if (!QDBusConnection::sessionBus().isConnected())
    {
        TError::SetCurrent(new TError(QObject::tr("Не удалось открыть D-Bus соединение.")));
        return false;
    }

    if (!QDBusConnection::sessionBus().registerService(_SERVICE_NAME))
    {
        TError::SetCurrent(new TError(QObject::tr("Не удалось зарегистрировать D-Bus сервис.")));
        return false;
    }

    if (!QDBusConnection::sessionBus().registerObject("/", &_notifier, QDBusConnection::ExportAllContents))
    {
        TError::SetCurrent(new TError(QObject::tr("Не удалось зарегистрировать D-Bus интерфейс.")));
        return false;
    }  

    _database.setHostName("localhost");
    _database.setDatabaseName(_DEFAULT_DB);
    _database.setUserName("root");
    _database.setPassword("root");

    if (!_database.open())
    {
        TError::SetCurrent(new TError(QObject::tr("Ошибка открытия БД '%1'").arg(_database.lastError().text())));
        return false;
    }

    if (!_database.tables().size())
    {
        QFile sql(":/targets_schema.sql");
        sql.open(QIODevice::ReadOnly | QIODevice::Text);
        QString sql_query(sql.readAll());
        QSqlQuery query(_database);
        if (!query.exec(sql_query))
        {
            TError::SetCurrent(new TError(QObject::tr("Ошибка БД '%1'").arg(query.lastError().text())));
            return false;
        }
    }

    return true;
}

bool TargetsStorage::AddTargets(const QVector<Target> &values)
{

    QStringList targets;

    foreach (const Target &value, values)
    {
        foreach (const Event &event, value._events)
        {
            {
                QSqlQuery query(_database);
                query.prepare(QStringLiteral("INSERT INTO targets(name, time, x, y, z, state) VALUES(:name, :time, :x, :y, :z, :state);"));
                query.bindValue(QStringLiteral(":name"), value._name);
                query.bindValue(QStringLiteral(":time"), event._time);
                query.bindValue(QStringLiteral(":x"), event._point._x);
                query.bindValue(QStringLiteral(":y"), event._point._y);
                query.bindValue(QStringLiteral(":z"), event._point._z);
                query.bindValue(QStringLiteral(":state"), event._state);
                if (!query.exec())
                {
                    TError::SetCurrent(new TError(QObject::tr("Ошибка БД '%1'").arg(query.lastError().text())));
                    return false;
                }
            }

            {
                QSqlQuery query(_database);
                query.prepare(QStringLiteral("INSERT INTO last_targets(name, time, x, y, z, state) "
                                             "VALUES (:name, :time, :x, :y, :z, :state) ON DUPLICATE KEY UPDATE time = :time, x = :x, y = :y, z = :z, state = :state;"));
                query.bindValue(QStringLiteral(":name"), value._name);
                query.bindValue(QStringLiteral(":time"), event._time);
                query.bindValue(QStringLiteral(":x"), event._point._x);
                query.bindValue(QStringLiteral(":y"), event._point._y);
                query.bindValue(QStringLiteral(":z"), event._point._z);
                query.bindValue(QStringLiteral(":state"), event._state);
                if (!query.exec())
                {
                    TError::SetCurrent(new TError(QObject::tr("Ошибка БД '%1'").arg(query.lastError().text())));
                    return false;
                }
            }
        }

        targets<<value._name;
    }

    _notifier.TargetsAdded(targets);

    return true;
}
