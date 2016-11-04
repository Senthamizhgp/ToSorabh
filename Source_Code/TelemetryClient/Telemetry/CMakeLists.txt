#
if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)
	cmake_minimum_required (VERSION 2.8.11)
else()
	cmake_minimum_required (VERSION 2.8.5)
endif ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)

#
project (Telemetry)

#
#include (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)

#
include_directories (.)
include_directories (${SRC_DIR}/TelemetryClient/ConfigurationReader)
include_directories (${SRC_DIR}/TelemetryClient/TimerLib)
include_directories (${SRC_DIR}/TelemetryClient/SensorTelemetryController)
include_directories (${SRC_DIR}/TelemetryClient/DexProcessingUnit)
include_directories (${SRC_DIR}/TelemetryClient/PIR-PeopleCounter)
include_directories (${SRC_DIR}/TelemetryClient/RealSense-PeopleCounter)
include_directories (${SRC_DIR}/TelemetryClient/Bluetooth-PeopleCounter)
include_directories (${SRC_DIR}/TelemetryClient/WiFi-PeopleCounter/include)
include_directories (${PROJ_DIR}/Lib/HLAPI/Include)
include_directories (${OpenCV_INCLUDE_DIRS})
include_directories (${RSSDK_INCLUDE_DIRS})	
include_directories (${RSSDK_COMMON_INCLUDE_DIRS})	
include_directories (${LIBXML2_INCLUDE_DIR})
if (WINDOWS OR WINRT)
	include_directories (${LIBICONV_INCLUDE_DIR})
endif (WINDOWS OR WINRT)
include_directories (${MQTT_INCUDE_DIR})

#
file (GLOB HEADERS *.h)
file (GLOB SOURCES *.cpp)

#
if (WINDOWS)
	if (RSSDK_DIR)
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DREALSENSE")
	endif (RSSDK_DIR)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32")
endif (WINDOWS)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	# disable 4127 warning
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd\"4127\" /Wall /GL /Gy /W4 /Zi /sdl /errorReport:prompt /Oi /EHsc /nologo")
	set (CMAKE_SHARED_LINKER_FLAGS "/LTCG:incremental")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIE -fstack-protector -Wall -Werror -Wextra -Wfloat-equal -Wundef -Wpointer-arith -Wcast-align -Wconversion -Wunreachable-code -std=c++11 -pthread")
endif ()

#
add_executable(${PROJECT_NAME} 
				${HEADERS} ${SOURCES})

#
if (WINDOWS OR WINRT)
	if (NOT TARGET HLAPI)					
		add_library (HLAPI SHARED IMPORTED)
		set_property (TARGET HLAPI PROPERTY IMPORTED_IMPLIB 
						${PROJ_DIR}/Lib/HLAPI/Lib/HLAPI.lib)
	endif (NOT TARGET HLAPI)
	
	if (Qt5Core_FOUND)
		#Link with Qt5 
		target_link_libraries (${PROJECT_NAME} 
								Qt5::Core 
								Qt5::Sql 
								HLAPI 
								ConfigurationReader 
								PIR 
								SensorTelemetryController
								DexProcessingUnit 
								${MQTT_LIBRARIES})
	elseif (QT4_FOUND)
		target_link_libraries (${PROJECT_NAME} 
								${QT_LIBRARIES} 
								HLAPI 
								ConfigurationReader
								PIR 
								SensorTelemetryController 
								DexProcessingUnit 
								${MQTT_LIBRARIES})
	endif (Qt5Core_FOUND)
	
	if(RSSDK_DIR)					  
		target_link_libraries(${PROJECT_NAME}  
								RealSensePeopleCounter)		
	endif()	
endif (WINDOWS OR WINRT)

#
if (UNIX)
	if (Qt5Core_FOUND)
		#Link with Qt5 
		target_link_libraries (${PROJECT_NAME} 
								Qt5::Core 
								Qt5::Sql 
								${HLAPI_LIBRARIES}
								${LLAPI_LIBRARIES}
								ConfigurationReader
								TimerLib
								rt
								PIR 
								SensorTelemetryController 
								DexProcessingUnit
								wifi	
								Bluetooth
								pcap
								${MQTT_LIBRARIES})
	elseif (QT4_FOUND)
		target_link_libraries (${PROJECT_NAME} 
								${QT_LIBRARIES} 
								${HLAPI_LIBRARIES} 
								${LLAPI_LIBRARIES}
								ConfigurationReader
								TimerLib
								rt
								PIR 
								SensorTelemetryController 
								DexProcessingUnit 
								wifi
								Bluetooth
								pcap
								${MQTT_LIBRARIES})
	endif (Qt5Core_FOUND)
endif (UNIX)

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR}) 
	#
	get_target_property(QtCore_location Qt5::Core LOCATION)
	get_filename_component(Qt_Location ${QtCore_location} DIRECTORY)
	get_filename_component(Qt_Location ${Qt_Location} DIRECTORY)
	
	#	
	FILE(GLOB XML
			${PROJ_DIR}/xml/*.xml
			${PROJ_DIR}/xml/*.xsd
			${PROJ_DIR}/Source_Code/TelemetryClient/ConfigurationReader/ConfigFiles/*.txt
			${PROJ_DIR}/Source_Code/TelemetryClient/ConfigurationReader/ConfigFiles/*.xml
			${PROJ_DIR}/Source_Code/TelemetryClient/ConfigurationReader/ConfigFiles/*.xsd)			
	INSTALL(FILES ${XML} DESTINATION ${BIN_DIR})
	
	if (WINDOWS)
		FILE(GLOB DLL 
				${Qt_Location}/bin/Qt5Core.dll
				${Qt_Location}/bin/Qt5Sql.dll
				${HLAPI_DLL}
				${LLAPI_DLL}
				${MQTT_DLL}
				${LIBXML2_DLL}
				${OPEN_SSL_DLL})

		INSTALL(FILES ${DLL} DESTINATION ${BIN_DIR})
		install(DIRECTORY ${Qt_Location}/plugins/sqldrivers DESTINATION ${BIN_DIR})
	endif (WINDOWS)

	if (UNIX)
		#
		#file(GLOB_RECURSE OTHER_SO "${PROJ_DIR}/*.so")
		#INSTALL(FILES ${OTHER_SO} DESTINATION ${BIN_DIR}) 
	endif (UNIX)
	
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${BIN_DIR})
	
endif (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
