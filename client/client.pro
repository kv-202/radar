
QT+=sql
QT+=dbus
CONFIG+=c++11
TARGET=client
TEMPLATE=app
SOURCES=main.cpp \
    ../terror.cpp \
    clienttargetsstorage.cpp \
    targetsstoragenotifiersubscriber.cpp \
    targetsservice.cpp

HEADERS=../terror.h \
    ../target.h \
    ../config.h \
    clienttargetsstorage.h \
    targetsstoragenotifiersubscriber.h \
    targetsservice.h

DISTFILES += \
    ../targets_schema.sql

RESOURCES += \
    client.qrc
