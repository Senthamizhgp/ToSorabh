# Cmake minimum required version
if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)
	cmake_minimum_required (VERSION 2.8.11)
else ()
	cmake_minimum_required (VERSION 2.8.5)
endif ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)

# Maps to a solution file Intel_Vending_Smart_Shelf.sln
project (Intel_Vending_Smart_Shelf)

# Load required enviroment variables to build Telemetry application
include (config.cmake)

# Add a subdirectory to the build
add_subdirectory (${SRC_DIR}/TelemetryClient/ConfigurationReader)
add_subdirectory (${SRC_DIR}/TelemetryClient/PIR-PeopleCounter)

if (UNIX)
	add_subdirectory (${SRC_DIR}/TelemetryClient/TimerLib)
endif (UNIX)
	
add_subdirectory (${SRC_DIR}/TelemetryClient/SensorTelemetryController)
add_subdirectory (${SRC_DIR}/TelemetryClient/DexProcessingUnit)

if (RSSDK_DIR)	# this is only for windows
	add_subdirectory (${SRC_DIR}/TelemetryClient/RealSense-PeopleCounter)
else ()
	message(WARNING "RealSense SDK not found.")
endif (RSSDK_DIR)

if (UNIX)
	add_subdirectory (${SRC_DIR}/TelemetryClient/Bluetooth-PeopleCounter)
	add_subdirectory (${SRC_DIR}/TelemetryClient/WiFi-PeopleCounter)
endif (UNIX)

add_subdirectory (${SRC_DIR}/TelemetryClient/Telemetry)
