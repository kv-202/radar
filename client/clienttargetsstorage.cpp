
#include "../terror.h"
#include "../config.h"
#include "QString"
#include "clienttargetsstorage.h"
#include <algorithm>

static const QString _DB_DT_FORMAT = QStringLiteral("yyyy-MM-dd hh:mm:ss.zzz");

void ClientTargetsStorage::TargetsAddedEmitter(QStringList targets)
{
    TargetsAdded(targets);
}

ClientTargetsStorage::ClientTargetsStorage():
    _database(QSqlDatabase::addDatabase("QMYSQL", "core_app")),
    _notifier(_SERVICE_NAME, "/", QDBusConnection::sessionBus())
{
    QObject::connect(&_notifier, &TargetsStorageNotifierSubscriber::TargetsAdded, this, &ClientTargetsStorage::TargetsAddedEmitter);
}

bool ClientTargetsStorage::Open(const QString &file)
{
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

bool ClientTargetsStorage::GetTargets(const QStringList &targets, const QDateTime &from, const QDateTime &to, QVector<Target>& values)
{
    values.clear();

    QString sqlQuery("SELECT name, x, y, z, state, time FROM targets");

    if (targets.size() || !from.isNull() || !to.isNull())
        sqlQuery.append(" WHERE");


    if (!from.isNull())
    {
        sqlQuery.append(QString(" time > \"%1\"").arg(from.toString(_DB_DT_FORMAT)));

        if (targets.size() || !to.isNull())
            sqlQuery.append(" AND");
    }

    if (!to.isNull())
    {
        sqlQuery.append(QString(" time < \"%1\"").arg(to.toString(_DB_DT_FORMAT)));

        if (targets.size() || !to.isNull())
            sqlQuery.append(" AND");
    }

    if (targets.size())
    {
        sqlQuery.append(QString(" name IN ("));
        sqlQuery.append("\"" + *(targets.begin()) + "\"");

        for (auto it = targets.begin() + 1; it != targets.end(); ++it)
            sqlQuery.append(", \"" + *it + "\"");

        sqlQuery.append(")");
    }

    sqlQuery.append(";");

    QSqlQuery query(_database);
    if (!query.exec(sqlQuery))
    {
        TError::SetCurrent(new TError(QObject::tr("Ошибка БД '%1'").arg(query.lastError().text())));
        return false;
    }

    while (query.next())
    {
        Event e;
        e._point._x = query.value(1).toInt();
        e._point._y = query.value(2).toInt();
        e._point._z = query.value(3).toInt();
        e._state = static_cast<Event::State>(query.value(4).toInt());
        e._time = query.value(5).toDateTime();

        QString name = query.value(0).toString();

        QVector<Target>::Iterator it = std::find_if(values.begin(), values.end(), [&name] (const Target& a) { return a._name == name; } );
        if (it == values.end())
        {
            Target t;
            t._name = name;
            t._events.push_back(e);
            values.push_back(t);
        }
        else
            it->_events.push_back(e);
    }

    return true;
}

bool ClientTargetsStorage::GetLastTargets(const QStringList &targets, QVector<Target>& values)
{

    values.clear();

    QString sqlQuery("SELECT name, x, y, z, state, time FROM last_targets");

    if (targets.size())
    {
        sqlQuery.append(QString(" WHERE name IN ("));
        sqlQuery.append("\"" + *(targets.begin()) + "\"");

        for (auto it = targets.begin() + 1; it != targets.end(); ++it)
            sqlQuery.append(", \"" + *it + "\"");

        sqlQuery.append(")");
    }

    sqlQuery.append(";");

    QSqlQuery query(_database);
    if (!query.exec(sqlQuery))
    {
        TError::SetCurrent(new TError(QObject::tr("Ошибка БД '%1'").arg(query.lastError().text())));
        return false;
    }

    while (query.next())
    {
        Event e;
        e._point._x = query.value(1).toInt();
        e._point._y = query.value(2).toInt();
        e._point._z = query.value(3).toInt();
        e._state = static_cast<Event::State>(query.value(4).toInt());
        e._time = query.value(5).toDateTime();

        Target t;
        t._name = query.value(0).toString();
        t._events.push_back(e);
        values.push_back(t);
    }

    return true;
}
