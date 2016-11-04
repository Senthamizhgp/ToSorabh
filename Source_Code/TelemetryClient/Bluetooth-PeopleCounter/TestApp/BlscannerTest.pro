TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QT    += core
QT       +=  dbus

QT       +=  bluetooth

SOURCES += main.cpp
INCLUDEPATH +="../../Bluetooth-PeopleCounter"
INCLUDEPATH +="../../../../Lib/LibXML/Include"
INCLUDEPATH +="../../ConfigurationReader"

LIBS += -L ../../../../bin \
    -lQt5Bluetooth \
    -lQt5Core \
    -lQt5DBus \
    -lQt5Concurrent \
    -lQt5Sql \
    -lBluetooth \
    -lConfigurationReader
