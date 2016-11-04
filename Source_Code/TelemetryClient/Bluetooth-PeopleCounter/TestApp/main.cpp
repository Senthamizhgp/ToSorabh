#include <iostream>
#include <bluetooth.h>
using namespace std;

int main(int argc, char *argv[])
{



    Bluetooth b;
    char* sql=  "INSERT INTO intel.Bluetooth_Counter	(DEVICE_NAME, MAC_ADDR, RSSI,SENSOR_NAME, SENSOR_ID, TIMESTAMP)VALUES 	(:DEVICE_NAME, :MAC_ADDR,:RSSI, :SENSOR_NAME, :SENSOR_ID, :TIMESTAMP)";
    arguments arg[5] =
    {
        { ":DEVICE_NAME","Test" },
        { ":RSSI",std::to_string(2345) },
        { ":MAC_ADDR", "dataList[i]->mac().toStdString()" },
        { ":SENSOR_NAME", "localDevice->name().toStdString()"  },
        { ":SENSOR_ID"," (localDevice->address().toString()).toStdString()"  },
    };

    cout << "Hello World!" << endl;
    return 0;
}
