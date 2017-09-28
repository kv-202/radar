
#include "targetssource.h"
#include "../terror.h"
#include "../config.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

TargetsSource::TargetsSource(const QString &fileName):
    _timer(0),
    _fileName(fileName)
{
    _timer = startTimer(100);
}

TargetsSource::~TargetsSource()
{
    if (_timer)
        killTimer(_timer);
}

bool TargetsSource::GetTargets(QVector<Target>& values)
{
    values.clear();

    QFile file(_fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        TError::SetCurrent(new TError(QObject::tr("Не удалось открыть файл %1.").arg(_fileName)));
        return false;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    if (doc.isNull())
    {
        TError::SetCurrent(new TError(QObject::tr("Неверный формат файла: %1.").arg(error.errorString())));
        return false;
    }

    QJsonArray jsonValues = doc.array();
    foreach (const QJsonValue &jsonValue, jsonValues)
    {
        QJsonObject obj = jsonValue.toObject();
        if (obj["id"].isNull())
        {
            TError::SetCurrent(new TError(QObject::tr("У одного из объектов не идентификатора.")));
            return false;
        }

        QString id = obj["id"].toString();

        if (obj["pos"].isNull())
        {
            TError::SetCurrent(new TError(QObject::tr("У объекта %1 нет атрибута pos.").arg(id)));
            return false;
        }

        QJsonObject pos = obj["pos"].toObject();

        if (pos["x"].isNull())
        {
            TError::SetCurrent(new TError(QObject::tr("У объекта %1 нет атрибута x.").arg(id)));
            return false;
        }

        if (pos["y"].isNull())
        {
            TError::SetCurrent(new TError(QObject::tr("У объекта %1 нет атрибута y.").arg(id)));
            return false;
        }

        if (pos["z"].isNull())
        {
            TError::SetCurrent(new TError(QObject::tr("У объекта %1 нет атрибута z.").arg(id)));
            return false;
        }

        if (obj["state"].isNull())
        {
            TError::SetCurrent(new TError(QObject::tr("У объекта %1 нет атрибута state.").arg(id)));
            return false;
        }

        if (obj["ts"].isNull())
        {
            TError::SetCurrent(new TError(QObject::tr("У объекта %1 нет атрибута ts.").arg(id)));
            return false;
        }

        QVector<QPair<Event::State, QString>> states;
        states<<qMakePair(Event::Found, QString("FOUND"));
        states<<qMakePair(Event::Leading, QString("LEADING"));
        states<<qMakePair(Event::Lost, QString("LOST"));

        QString state = obj["state"].toString().toUpper();

        auto it = std::find_if(states.begin(), states.end(),
                               [&state] (const QPair<Event::State, QString> &a) { return a.second == state; });

        if (it == states.end())
        {
            TError::SetCurrent(new TError(QObject::tr("У объекта %1 неверное значение атрибута state.").arg(id)));
            return false;
        }

        Event e;
        e._state = it->first;
//        e._time = QDateTime::fromString(pos["ts"].toString(), _DT_FORMAT);
        e._time = QDateTime::currentDateTime();
        e._point = { pos["x"].toInt(), pos["y"].toInt(), pos["z"].toInt() };

        auto value = std::find_if(values.begin(), values.end(),
                                  [&id] (const Target& a) { return a._name == id; } );

        if (value == values.end())
        {
            Target t;
            t._name = id;
            t._events.push_back(e);
            values.push_back(t);
        }
        else
            value->_events.push_back(e);
    }

    return true;
}

void TargetsSource::timerEvent(QTimerEvent */*e*/)
{
    TargetsReady();
}
