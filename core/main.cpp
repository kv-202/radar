
#include "../target.h"
#include "../terror.h"
#include "../config.h"
#include "targetssource.h"
#include "targetsservice.h"
#include "targetsstorage.h"
#include <QCoreApplication>
#include <iostream>

#define _TARGET_DEV "/home/kv_202/Tmp/TargetsDev.json"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    TargetsStorage storage;
    if (!storage.Open(_DEFAULT_DB))
    {
        std::cout<<TError::GetCurrent()->Msg().toStdString()<<std::endl;
        return 1;
    }

    TargetsSource source(_TARGET_DEV);
    TargetsService service(source, storage);

    return app.exec();
}
