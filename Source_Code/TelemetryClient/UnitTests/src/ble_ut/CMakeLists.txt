cmake_minimum_required(VERSION 3.0.0)

project(ble_ut)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(LINUX TRUE)
endif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

set(CMAKE_VERBOSE_MAKEFILE true)

find_package(Qt5 COMPONENTS Core Sql Bluetooth REQUIRED)
if (NOT (${Qt5Core_FOUND} OR ${Qt5Sql_FOUND} OR ${Qt5Bluetooth_FOUND} OR ${Qt5DBus_FOUND} OR ${Qt5Concurrent}))
	message( SEND_ERROR "Can not find Qt5 components")
endif (NOT (${Qt5Core_FOUND} OR ${Qt5Sql_FOUND} OR ${Qt5Bluetooth_FOUND} OR ${Qt5DBus_FOUND} OR ${Qt5Concurrent}))

include_directories(../../include)
include_directories(../../../../../Lib/HLAPI/Include)
include_directories(../../../Bluetooth-PeopleCounter)
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Sql_INCLUDE_DIRS})
include_directories(${Qt5Bluetooth_INCLUDE_DIRS})
include_directories(${Qt5DBus_INCLUDE_DIRS})
include_directories(${Qt5Concurrent_INCLUDE_DIRS})

link_directories(../../lib)

file(GLOB ALL_HEADERS 
						../../include/common_ut.h
						../../../Bluetooth-PeopleCounter/bluetooth.h
						../../../Bluetooth-PeopleCounter/dataobject.h)
file(GLOB ALL_SOURCES
						../common_ut.cpp
						../../../Bluetooth-PeopleCounter/bluetooth.cpp
						../../../Bluetooth-PeopleCounter/dataobject.cpp
						ble_ut.cpp)

if (WINDOWS)
  set(CMAKE_CXX_FLAGS "/Wall /Gy /W4 /Zi /sdl /errorReport:prompt /Oi /EHsc /nologo -DUNIT_TEST -D_CRT_SECURE_NO_WARNINGS -DDLL_PUBLIC -DDLL_LOCAL")
endif (WINDOWS)

if (LINUX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -DUNIT_TEST -D_CRT_SECURE_NO_WARNINGS -DDLL_PUBLIC -DDLL_LOCAL")
endif (LINUX)

add_executable(${PROJECT_NAME}
						${ALL_HEADERS}
						${ALL_SOURCES})
								
target_link_libraries(${PROJECT_NAME}
						cppunit
						${Qt5Core_LIBRARIES}
						${Qt5Sql_LIBRARIES}
						${Qt5Bluetooth_LIBRARIES}
						${Qt5Concurrent_LIBRARIES}
						${Qt5DBus_LIBRARIES})
						
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR})

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR}) 

	if (WINDOWS)
		get_target_property(QtCore_location Qt5::Core LOCATION)
		get_filename_component(Qt_Location ${QtCore_location} DIRECTORY)
		get_filename_component(Qt_Location ${Qt_Location} DIRECTORY)

		install(FILES "${Qt_Location}/bin/Qt5Core.dll" DESTINATION "${CMAKE_SOURCE_DIR}/bin/") 
		install(FILES "${Qt_Location}/bin/Qt5Sql.dll" DESTINATION "${CMAKE_SOURCE_DIR}/bin/")
		install(FILES "${Qt_Location}/bin/Qt5Bluetooth.dll" DESTINATION "${CMAKE_SOURCE_DIR}/bin/")
		install(FILES "${Qt_Location}/bin/Qt5Concurrent.dll" DESTINATION "${CMAKE_SOURCE_DIR}/bin/")
		install(FILES "${Qt_Location}/bin/Qt5DBus.dll" DESTINATION "${CMAKE_SOURCE_DIR}/bin/")
		install(FILES "${CMAKE_SOURCE_DIR}/../lib/win32/libmysql.dll" DESTINATION "${CMAKE_SOURCE_DIR}/bin/")
	endif (WINDOWS)
	
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/bin")
endif (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})