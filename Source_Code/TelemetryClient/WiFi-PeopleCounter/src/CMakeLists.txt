#
if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)
	cmake_minimum_required (VERSION 2.8.11)
else()
	cmake_minimum_required (VERSION 2.8.5)
endif ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)

#
project(wifi)

#
#include (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)

#
include_directories(../include)
include_directories(../libs/config_lib/include)
include_directories(../libs/hashtable/include)
include_directories(../libs/pkt_capture/include)                   
include_directories(../libs/pkt_parser/include)
include_directories(../libs/channel_selector/include)
include_directories(../libs/db-connector)

#
file(GLOB SOURCES *.c)

#
set(CMAKE_C_FLAGS "-g -fPIC -Werror -Wall -Wextra -Wmissing-prototypes -Wfloat-equal -Wundef -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wconversion -Wunreachable-code -fstack-protector")

#
add_library(${PROJECT_NAME} SHARED 
			${SOURCES})

target_link_libraries(${PROJECT_NAME} 
						channelselector
						configlib
						dbconnector
						hashtable
						pktcapture
						pktparser
						radiotap
						ConfigurationReader
						${Qt5Core_LIBRARIES}
						${Qt5Sql_LIBRARIES}
						${LIBXML2_LIBRARIES})

#						
install (TARGETS ${PROJECT_NAME} 
			LIBRARY DESTINATION ${BIN_DIR})
