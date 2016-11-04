# Cmake minimum required version
if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)
	cmake_minimum_required (VERSION 2.8.11)
else ()
	cmake_minimum_required (VERSION 2.8.5)
endif ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)

# Maps to a solution file Bluetooth.sln
project (Bluetooth)

# Initialize the AUTOMOC property for this target
set (CMAKE_AUTOMOC true)

#
include_directories (.)
include_directories (${SRC_DIR}/TelemetryClient/ConfigurationReader)
include_directories (${SRC_DIR}/TelemetryClient/Telemetry)
if (LIBXML2_INCLUDE_DIR)
	include_directories (${LIBXML2_INCLUDE_DIR})
else ()
	message (SEND_ERROR "LIBXML2_INCLUDE_DIR not set in config.cmake")
endif (LIBXML2_INCLUDE_DIR)

#
file (GLOB HEADERS *.h)
file (GLOB SOURCES *.cpp
${SRC_DIR}/TelemetryClient/Telemetry/DBHandler.cpp)

#
set (CMAKE_CXX_FLAGS "-pipe -g -Wall -W -D_REENTRANT -fPIC")

#
add_library (${PROJECT_NAME} SHARED 
				${HEADERS} ${SOURCES})

#
add_dependencies(${PROJECT_NAME} ConfigurationReader)

#
if (${Qt4_FOUND})
	# Find Required Qt4 Packages
	find_package (Qt4 REQUIRED QtCore QtBluetooth QtDBus)
	include (${QT_USE_FILE})
else ()
	# Find Required Qt5 Packages
	find_package (Qt5Core REQUIRED)
	find_package (Qt5Bluetooth REQUIRED)
	find_package (Qt5Sql REQUIRED)
	find_package (Qt5DBus REQUIRED)
endif (${Qt4_FOUND})

#
if (Qt5Core_FOUND)
	target_link_libraries (${PROJECT_NAME} 
							ConfigurationReader
							Qt5::Core 
							Qt5::Sql 
							Qt5::Bluetooth
							Qt5::DBus)
elseif (QT4_FOUND)
	target_link_libraries (${PROJECT_NAME} 
							ConfigurationReader
							${QT_LIBRARIES})
endif (Qt5Core_FOUND)

message ("Bin_DIR is ${BIN_DIR}")

if (WINDOWS OR WINRT)
	install (TARGETS ${PROJECT_NAME}
				RUNTIME DESTINATION ${BIN_DIR})
endif (WINDOWS OR WINRT)

if (UNIX)
	install (TARGETS ${PROJECT_NAME} 
			LIBRARY DESTINATION ${BIN_DIR})
endif(UNIX)

