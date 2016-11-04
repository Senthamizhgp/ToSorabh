#
if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)
	cmake_minimum_required (VERSION 2.8.11)
else()
	cmake_minimum_required (VERSION 2.8.5)
endif ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)

#
project (SensorTelemetryController)

#
#include (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)

#
include_directories (.)
include_directories (${SRC_DIR}/TelemetryClient/ConfigurationReader)
include_directories (${SRC_DIR}/TelemetryClient/Telemetry)
include_directories (${PROJ_DIR}/Lib/HLAPI/Include)
include_directories (${LIBXML2_INCLUDE_DIR})
if (WINDOWS OR WINRT)
	include_directories (${LIBICONV_INCLUDE_DIR})
endif (WINDOWS OR WINRT)

#
file (GLOB HEADERS *.h)
file (GLOB SOURCES *.cpp
					${SRC_DIR}/TelemetryClient/Telemetry/DBHandler.cpp)

#
if (WINDOWS)
	set (CMAKE_CXX_FLAGS "/Wall /GL /Gy /W4 /Zi /sdl /errorReport:prompt /Oi /EHsc /nologo")
	set (CMAKE_SHARED_LINKER_FLAGS "/LTCG:incremental")
elseif (UNIX)
	set (CMAKE_CXX_FLAGS "-g -Wall -Wextra -std=c++11")
endif (WINDOWS)

#
add_library(${PROJECT_NAME} SHARED 
			${HEADERS} ${SOURCES})

#
if (WINDOWS OR WINRT)	
	if (NOT TARGET HLAPI)
		add_library (HLAPI SHARED IMPORTED)
		set_property (TARGET HLAPI PROPERTY IMPORTED_IMPLIB 
						${HLAPI_LIBRARIES})
	endif(NOT TARGET HLAPI)
	
	if (Qt5Core_FOUND)
		#Link with Qt5 
		target_link_libraries (${PROJECT_NAME} 
								ConfigurationReader
								HLAPI
								Qt5::Core 
								Qt5::Sql)
	elseif (QT4_FOUND)
		#Link with Qt4 
		target_link_libraries (${PROJECT_NAME} 
								ConfigurationReader
								HLAPI
								${QT_LIBRARIES})
	endif (Qt5Core_FOUND)
	
	#install target library to given path
	install (TARGETS ${PROJECT_NAME} 
				RUNTIME DESTINATION ${BIN_DIR})
endif (WINDOWS OR WINRT)

#
if (UNIX)
	if (Qt5Core_FOUND)
		#Link with Qt5 
		target_link_libraries (${PROJECT_NAME} 
								ConfigurationReader
								${HLAPI_LIBRARIES}
								Qt5::Core 
								Qt5::Sql)
	elseif (QT4_FOUND)
		target_link_libraries (${PROJECT_NAME} 
								ConfigurationReader
								${HLAPI_LIBRARIES}
								${QT_LIBRARIES})
	endif (Qt5Core_FOUND)
	
	install (TARGETS ${PROJECT_NAME} 
				LIBRARY DESTINATION ${BIN_DIR})
endif (UNIX)


