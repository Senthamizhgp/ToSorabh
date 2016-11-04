CC            = g++
INCPATH       = -I. -I../../src/WiFi-PeopleCounter/include -I../../src/WiFi-PeopleCounter/libs/channel_selector/include/ -I../../src/WiFi-PeopleCounter/libs/hashtable/include -I../../src/WiFi-PeopleCounter/libs/db-connector/

CFLAGS        = -g -Wall  -fpermissive -DUT=$(UT)

LIBS          = -lcppunit -L ../../bin -lConfigurationReader -lwifi -lQt5Sql -lQt5Core -lpcap -lm `mysql_config --cflags --libs`

first: all

all: 
	$(CC) $(INCPATH) $(CFLAGS) wifi_ut.cpp -o wifi_ut $(LIBS)

clean:
	rm wifi_ut
