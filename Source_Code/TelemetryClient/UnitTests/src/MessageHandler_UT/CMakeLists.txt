cmake_minimum_required(VERSION 3.0.0)

project(MessageHandler_UT)

set (PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(LINUX TRUE)
endif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

set(CMAKE_VERBOSE_MAKEFILE true)

find_package(Qt5 COMPONENTS Core Sql REQUIRED)
if (NOT (${Qt5Core_FOUND} OR ${Qt5Sql_FOUND}))
	message( SEND_ERROR "Can not find Qt5 components")
endif (NOT (${Qt5Core_FOUND} OR ${Qt5Sql_FOUND}))

include_directories(../../include)
include_directories(../../../../../Lib/LibXML/Include)
include_directories(../../../../../Lib/PahoMQTT/Include)
include_directories(../../../ConfigurationReader)
include_directories(../../../Telemetry)
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Sql_INCLUDE_DIRS})


link_directories(../../lib)
link_directories(../../../../../Lib/PahoMQTT/Lib
				 ../../../../../bin)

file(GLOB ALL_HEADERS 						
						../../../Telemetry/MessageHandler.h)

file(GLOB ALL_SOURCES
						../../../Telemetry/MessageHandler.cpp
						../../../Telemetry/MessageQueue.cpp
						MessageHandler_UT.cpp)

if (WINDOWS)
	set(LIBXML2_LIBRARIES libxml2)
	set(CMAKE_CXX_FLAGS "/Wall /Gy /W4 /Zi /sdl /errorReport:prompt /Oi /EHsc /nologo -DWIN32 -DUNIT_TEST")
endif (WINDOWS)

if (LINUX)
	set (LIBXML2_LIBRARIES ${PROJ_DIR}/Lib/LibXML/Lib/libxml2.lib)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -DUNIT_TEST -std=c++11")
endif (LINUX)

add_executable(${PROJECT_NAME}
						${ALL_HEADERS}
						${ALL_SOURCES})
								
target_link_libraries(${PROJECT_NAME}
						cppunit
						paho-mqtt3as	
						ConfigurationReader					
						${Qt5Core_LIBRARIES}
						${Qt5Sql_LIBRARIES}
						${LIBXML2_LIBRARIES})


set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR})

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR}) 
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/bin")
endif (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
				

