#CMake minimum required version
cmake_minimum_required(VERSION 3.0.0)

# Maps to a solution file Intel_Vending_Smart_Shelf_UT.sln
project(Intel_Vending_Smart_Shelf_UT)

# Find OS for which CMake is to build
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(WINDOWS TRUE)
	set(CMAKE_CONFIGURATION_TYPES "Release")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(LINUX TRUE)
endif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

# set system environment variable CMAKE_PREFIX_PATH to <Qt installation path >
#as mentioned in CMake user guide
#set(CMAKE_PREFIX_PATH "")

#
set(CMAKE_VERBOSE_MAKEFILE true)

# Add subdirectory
add_subdirectory(src/PIR_UT)
add_subdirectory(src/ble_ut)
add_subdirectory(src/WiFiPeopleCounter-UT)
add_subdirectory(src/ConfigurationReader_UT)
add_subdirectory(src/SensorTelemetryController_UT)
add_subdirectory(src/Datatranslator_UT)
add_subdirectory(src/DEX_Converter_UT)
add_subdirectory(src/IDEX_UT)
add_subdirectory(src/MessageHandler_UT)
add_subdirectory(src/PublishTableController_UT)
add_subdirectory(src/Transaction_UT)
add_subdirectory(src/DataTransmissionController_UT)

file(COPY ../../../bin/TelemetryConfiguration.xml DESTINATION ../bin)
file(COPY ../../../bin/TelemetryConfiguration.xsd DESTINATION ../bin)
file(COPY ../../../bin/DEXConfiguration.xml DESTINATION ../bin)
file(COPY ../../../bin/libConfigurationReader.so DESTINATION ../bin)
file(COPY ../../../bin/libpaho-mqtt3as.so DESTINATION ../bin)
