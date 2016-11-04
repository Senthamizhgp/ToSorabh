#Cmake minimum required version
cmake_minimum_required(VERSION 3.0.0)

#
project(test_pir)

# Find OS for which CMake is to build
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(WINDOWS TRUE)
	set(CMAKE_CONFIGURATION_TYPES "Release")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(LINUX TRUE)
endif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

#
set(CMAKE_VERBOSE_MAKEFILE true)

set(CMAKE_PREFIX_PATH "D:\\Hari\\Programs\\Qt\\5.7\\msvc2015")

#
find_package(Qt5 COMPONENTS Core Sql REQUIRED)
if (NOT (${Qt5Core_FOUND} OR ${Qt5Sql_FOUND}))
	message( SEND_ERROR "Can not find Qt5 components")
endif (NOT (${Qt5Core_FOUND} OR ${Qt5Sql_FOUND}))

if (LINUX)
	find_package(LibXml2 QUIET REQUIRED)
	if (NOT ${LIBXML2_FOUND})
		message(SEND_ERROR "Can not find LibXml2")
	endif (NOT ${LIBXML2_FOUND})
endif (LINUX)

#
include_directories(../)
include_directories(../../ConfigurationReader)
include_directories(../../../../Lib/HLAPI/Include)
include_directories(../../../../Lib/LibXML/Include)
include_directories(../../../../Lib/Libiconv/Include)

#
link_directories(../../../../Lib/HLAPI/Lib)
link_directories(../../build/PIR-PeopleCounter/${CMAKE_CONFIGURATION_TYPES})
link_directories(../../build/ConfigurationReader/${CMAKE_CONFIGURATION_TYPES})

#
file(GLOB ALL_HEADERS *.h)
file(GLOB ALL_SOURCES *.cpp)

#
if (WINDOWS)
	set(LIBXML2_LIBRARIES libxml2)
	
#	set(CMAKE_CXX_FLAGS "/Wall /GL /Gy /W4 /Zi /sdl /errorReport:prompt /Oi /EHsc /nologo")
#	set(CMAKE_SHARED_LINKER_FLAGS "/LTCG:incremental")
endif (WINDOWS)

if (LINUX)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")
endif (LINUX)

#
add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${ALL_HEADERS})

#
target_link_libraries(${PROJECT_NAME} HLAPI PIR ConfigurationReader)
