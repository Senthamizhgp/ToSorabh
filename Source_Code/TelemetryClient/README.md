CONTENTS OF THIS FILE
---------------------
1.Introduction
2.Development Environment
3.Folder Structure
4.Installation
5.Configuration
6.Troubleshooting

1.Introduction
--------------
	Intel Intelligent Vending and Smart Shelf compsrises of Telemetry Client and people counter applications using Wi-Fi, Bluetooth, and PIR (Passive Infrared) sensor. The main purpose of the Telemetry Clinet is to collect data from different database and publish it to the MQTT borker.

2.Development Environment
-------------------------
	This project support both Windows and Linux deevelopment environment. The following section covers in detail on how to setup the development environment for both Windows and Linux.

	Note:- The build system for both Windows and Linux uses CMake utility.

	Windows
	=======
	<<To fill>>

	Linux
	=====
	<<To fill>>

3.Folder Structure
------------------
	The folder structre for this project is as shown below.

	Intel_Intelligent_Vending_and_Smart_Shelf
		|-*bin
		|-*build
		|- include						
		....|-libiconv												
		....|-libxml						
		....|-MQTT							
		|-lib					
		....|-linux64
		....|-win32
		|-src
		....|-Bluetooth
		....|-ConfigurationReader
		....|-PIR
		....|-SensorTelemetryController
		....|-Telemetry
		....|-WiFi-PeopleCounter
		|-tests
		....|-*bin
		....|-*build
		....|include
		....|lib
		....|src
		|-xml
	
	*bin and *build - Temporary folder that contains final binaries and intermediate build files created using CMake.

4.Installation
--------------

	Windows
	=======
	- Unpack source
	- Update "CMAKE_PREFIX_PATH" in root CMakeLists.tx with Qt installation directory.
	- Create “build” directory on root folder.
	- On Windows: 
		Open “Developer Command Prompt for VS2015” and navigate to build directory created in step#2 and run “cmake ../” (inside build folder) to create VS solution files.
		
		Compile the code using Visual Studio solution and use INSTALL project in solution to copy binaries to bin folder.
		
	- On Linux: 
		Open terminal and navigate to build directory created in step#2 and run “cmake ../” to create Makefile.
	
		Run make && make install to compile and install binaries to bin folder.

5.Configuration
---------------
Explain TelemetryConfiguration.xml


6.Troubleshooting
-----------------


