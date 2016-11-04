#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <vector>
#include "QtCore/qstring.h"

class DataObject
{

public:
    DataObject();
    DataObject(const QString &name, const QString &mac, const QString &timestamp, const short &rssi);
   // DataObject(const QString &name, const QString &mac, const QString &timestamp, const short &rssi, const QString startTime);

    QString name();
    void setName(const QString &name);
    QString mac() const;
    void setMac(const QString &mac);
    short int  rssi();
    void setRssi(const short int  &rssi);
    QString timestamp() const;
    void setTimestamp(const QString &timestamp);
    QString startTime() ;
    void setStartTime(const QString &start_time);
    bool operator==(DataObject& rhs);

private:
    QString  a_name;
    QString  a_mac;
    QString  a_timestamp;
    QString  a_startTime;
    short int  a_rssi;
};
#endif // DATAOBJECT_H
