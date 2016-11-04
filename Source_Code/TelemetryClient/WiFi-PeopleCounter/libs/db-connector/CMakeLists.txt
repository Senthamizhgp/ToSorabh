#CMake minimum required version
cmake_minimum_required(VERSION 3.0.0)

#
project(dbconnector)

#
set(CMAKE_VERBOSE_MAKEFILE true)

#
find_package(Qt5 COMPONENTS Core Sql REQUIRED)
if (NOT (${Qt5Core_FOUND} OR ${Qt5Sql_FOUND}))
	message( SEND_ERROR "Can not find Qt5 components Core and Sql")
endif (NOT (${Qt5Core_FOUND} OR ${Qt5Sql_FOUND}))

#
include_directories(include)
include_directories(../../include)
include_directories (${SRC_DIR}/TelemetryClient/ConfigurationReader)
include_directories (${SRC_DIR}/TelemetryClient/Telemetry)
include_directories (${LIBXML2_INCLUDE_DIR})
include_directories(../../../../include)
include_directories(${Qt5Sql_INCLUDE_DIRS})

#
file(GLOB ALL_SOURCES *.cpp
	${SRC_DIR}/TelemetryClient/Telemetry/DBHandler.cpp)
file(GLOB ALL_HEADERS *.h)

#
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -std=c++11 -fPIC")

#
add_library(${PROJECT_NAME} STATIC 
			${ALL_SOURCES})
