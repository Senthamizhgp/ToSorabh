#Cmake minimum required version
cmake_minimum_required(VERSION 3.0.0)

#
project(wifi_ut)

#
set(CMAKE_VERBOSE_MAKEFILE true)

#
find_package(Qt5 COMPONENTS Core Sql REQUIRED)
if (NOT (${Qt5Core_FOUND} OR ${Qt5Sql_FOUND}))
	message( SEND_ERROR "Can not find Qt5 components Core and Sql")
endif (NOT (${Qt5Core_FOUND} OR ${Qt5Sql_FOUND}))

find_package(LibXml2 QUIET REQUIRED)
if (NOT ${LIBXML2_FOUND})
	message(SEND_ERROR "Can not find LibXml2")
endif (NOT ${LIBXML2_FOUND})

#find_package(CPPUnit REQUIRED)
#if (NOT ${CPPUnit_FOUND})
#	message(SEND_ERROR "Can not find CPPunit")
#endif (NOT ${CPPUnit_FOUND})

#
include_directories(
	/usr/include/libxml2
	../../../WiFi-PeopleCounter/libs/radiotap
       ../../../WiFi-PeopleCounter/include
       ../../../WiFi-PeopleCounter/libs/channel_selector/include
       ../../../WiFi-PeopleCounter/libs/config_lib/include
       ../../../WiFi-PeopleCounter/libs/db-connector
       ../../../WiFi-PeopleCounter/libs/hashtable/include
       ../../../WiFi-PeopleCounter/libs/pkt_capture/include
       ../../../WiFi-PeopleCounter/libs/pkt_parser/include
       ../../../ConfigurationReader/)


link_directories(../../../../../bin)

#
if (LINUX)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -DUT")
	set(CMAKE_C_FLAGS "-Wall -Wextra -DUT")
endif (LINUX)

#
file(GLOB ALL_SOURCES 
	../../../WiFi-PeopleCounter/libs/channel_selector/src/*.c
	../../../WiFi-PeopleCounter/libs/config_lib/src/*.cpp
	../../../WiFi-PeopleCounter/libs/db-connector/*.cpp
	../../../WiFi-PeopleCounter/libs/hashtable/src/*.c
	../../../WiFi-PeopleCounter/libs/pkt_capture/src/*.c
	../../../WiFi-PeopleCounter/libs/pkt_parser/src/*.c
	../../../WiFi-PeopleCounter/libs/radiotap/radiotap.c
	../../../WiFi-PeopleCounter/src/*.c
	./*.cpp)

#
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

target_link_libraries(${PROJECT_NAME}
			pcap
			cppunit
			rt
			ConfigurationReader
			${Qt5Sql_LIBRARIES}
			${LIBXML2_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR})

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR}) 
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/bin")
endif (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
