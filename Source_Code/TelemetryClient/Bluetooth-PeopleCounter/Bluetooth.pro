#-------------------------------------------------
#
# Project created by QtCreator 2016-07-05T12:00:02
#
#-------------------------------------------------

QT       -= core
QT       +=  dbus

QT       +=  bluetooth
TARGET = Bluetooth
TEMPLATE = lib
QMAKE_CXXFLAGS += -Wno-enum-compare

DEFINES += BLUETOOTH_LIBRARY


SOURCES += \
    bluetooth.cpp \
    dataobject.cpp \




HEADERS +=\   
    dataobject.h \
    bluetooth.h




unix {

    target.path = /usr/lib

    INSTALLS += target

}


unix {
    target.path = /usr/lib
    INSTALLS += target
}
INCLUDEPATH +=$$PWD/../../include
INCLUDEPATH +=$$PWD/../ConfigurationReader

LIBS += -L /opt/Intel/lib \
    -lQt5Bluetooth \
    -lQt5Core \
    -lQt5DBus \
    -lQt5Concurrent \
    -lQt5Sql \
    -lConfigurationReader \


LIBS +=/usr/lib/x86_64-linux-gnu/libxml2.so.2
LIBS +=/usr/lib/x86_64-linux-gnu/libxml2.so.2.9.3
