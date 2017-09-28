#ifndef TARGETSSOURCE_H
#define TARGETSSOURCE_H

#include "../target.h"
#include <QVector>
#include <QObject>
#include <QString>
#include <QTimerEvent>

class TargetsSource : public QObject {
    Q_OBJECT
signals:
    void TargetsReady();

public:
    TargetsSource(const QString &fileName);
    ~TargetsSource();

    bool GetTargets(QVector<Target>& values);

protected:
    virtual void timerEvent(QTimerEvent *e) override;

private:
    int _timer;
    QString _fileName;
};

#endif // TARGETSSOURCE_H
