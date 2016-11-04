cmake_minimum_required(VERSION 3.0.0)

project(PublishTableController_UT)

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


if (LINUX)
	find_package(LibXml2 QUIET REQUIRED)
	if (NOT ${LIBXML2_FOUND})
		message(SEND_ERROR "Can not find LibXml2")
	endif (NOT ${LIBXML2_FOUND})
endif (LINUX)

include_directories(../../include)
include_directories(../../../../../Lib/HLAPI/Include)
include_directories(../../../../../Lib/LibXML/Include)
include_directories(../../../Telemetry)
include_directories(../../../ConfigurationReader)
include_directories(../../../DexProcessingUnit)
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Sql_INCLUDE_DIRS})

link_directories(
			../../lib
			../../../../../Lib/HLAPI/Lib
			../../../../../Lib/LLAPI/Lib
			../../../../../bin)

file(GLOB ALL_HEADERS 	
						../../../Telemetry/PublishTableController.h		
						../../../Telemetry/IDEX.h					.
						../../../Telemetry/datatranslator.h
						Transaction_mock.h)												

file(GLOB ALL_SOURCES						
						../../../Telemetry/PublishTableController.cpp						
						../../../Telemetry/datatranslator.cpp
						../../../Telemetry/IDEX.cpp	
						Transaction_mock.cpp
						Publishtable_UT.cpp)

if (WINDOWS)
  set(CMAKE_CXX_FLAGS "/Wall /Gy /W4 /Zi /sdl /errorReport:prompt /Oi /EHsc /nologo -DUNIT_TEST")
endif (WINDOWS)

if (LINUX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -DUNIT_TEST -std=c++11")
endif (LINUX)

add_executable(${PROJECT_NAME}
						${ALL_HEADERS}
						${ALL_SOURCES})
								
target_link_libraries(${PROJECT_NAME}
						cppunit		
						ConfigurationReader
						DexProcessingUnit
						${Qt5Core_LIBRARIES}
						${Qt5Sql_LIBRARIES}
						${LIBXML2_LIBRARIES})
						
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR})

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR}) 

	if (WINDOWS)
		get_target_property(QtCore_location Qt5::Core LOCATION)
		get_filename_component(Qt_Location ${QtCore_location} DIRECTORY)
		get_filename_component(Qt_Location ${Qt_Location} DIRECTORY)

		install(FILES "${Qt_Location}/bin/Qt5Core.dll" DESTINATION "${CMAKE_SOURCE_DIR}/bin/") 
		install(FILES "${Qt_Location}/bin/Qt5Sql.dll" DESTINATION "${CMAKE_SOURCE_DIR}/bin/")
		install(FILES "${CMAKE_SOURCE_DIR}/../lib/win32/libmysql.dll" DESTINATION "${CMAKE_SOURCE_DIR}/bin/")
	endif (WINDOWS)
	
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/bin")
endif (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
