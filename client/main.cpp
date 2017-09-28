
#include "clienttargetsstorage.h"
#include "targetsservice.h"
#include "../config.h"
#include "../terror.h"
#include <QCoreApplication>
#include <iostream>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    ClientTargetsStorage storage;
    if (!storage.Open(_DEFAULT_DB))
    {
        std::cout<<TError::GetCurrent()->Msg().toStdString()<<std::endl;
        return 1;
    }

    TargetsService service(storage);

    return app.exec();
}
