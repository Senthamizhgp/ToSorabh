#
if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)
	cmake_minimum_required (VERSION 2.8.11)
else()
	cmake_minimum_required (VERSION 2.8.5)
endif ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)

#
project(configlib)

#
#include (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)

#
include_directories(include)
include_directories(../../include)
include_directories (${SRC_DIR}/TelemetryClient/ConfigurationReader)
include_directories (${LIBXML2_INCLUDE_DIR})
include_directories(../../../../include)


#
file(GLOB ALL_SOURCES src/*.cpp)

#
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -std=c++11 -fPIC")

#
add_library(${PROJECT_NAME} STATIC 
			${ALL_SOURCES})
