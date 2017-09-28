
QT+=sql
QT+=dbus
CONFIG+=c++11
TARGET=core
TEMPLATE=app
SOURCES=main.cpp \
    ../terror.cpp \
    targetssource.cpp \
    targetsservice.cpp \
    targetsstorage.cpp

HEADERS=../terror.h \
    ../target.h \
    ../config.h \
    targetssource.h \
    targetsservice.h \
    targetsstorage.h \
    targetsstoragenotifier.h

DISTFILES += \
    ../targets_schema.sql

RESOURCES += \
    core.qrc
