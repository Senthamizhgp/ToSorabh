#
if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)
	cmake_minimum_required (VERSION 2.8.11)
else()
	cmake_minimum_required (VERSION 2.8.5)
endif ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)

#
project (RealSensePeopleCounter)

# Version
set (RSSDK_VERSION 0)
file (STRINGS "${RSSDK_INCLUDE_DIRS}/pxcversion.h" _pxcversion_H_CONTENTS REGEX "#define PXC_VERSION_.*")
set (_RSSDK_VERSION_REGEX "([0-9]+)")
foreach (v MAJOR MINOR BUILD REVISION)
    if ("${_pxcversion_H_CONTENTS}" MATCHES ".*#define PXC_VERSION_${v} *${_RSSDK_VERSION_REGEX}.*")
		set(RSSDK_VERSION_${v} "${CMAKE_MATCH_1}")
    endif ()
endforeach ()
unset (_pxcversion_H_CONTENTS)
set (RSSDK_VERSION "${RSSDK_VERSION_MAJOR}.${RSSDK_VERSION_MINOR}.${RSSDK_VERSION_BUILD}.${RSSDK_VERSION_REVISION}")

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args(RSSDK
								  FOUND_VAR RSSDK_FOUND
								  REQUIRED_VARS RSSDK_LIBRARIES RSSDK_INCLUDE_DIRS
								  VERSION_VAR RSSDK_VERSION)

if (MSVC)
	set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMTD")
endif ()

#
include_directories (${SRC_DIR}/TelemetryClient/ConfigurationReader)
include_directories (${LIBXML2_INCLUDE_DIR})
include_directories (${LIBICONV_INCLUDE_DIR})
include_directories (${Qt5Sql_INCLUDE_DIRS})
include_directories (${OpenCV_INCLUDE_DIRS})
include_directories (${RSSDK_INCLUDE_DIRS})	
include_directories (${RSSDK_COMMON_INCLUDE_DIRS})		
include_directories (${SRC_DIR}/TelemetryClient/Telemetry)
link_directories (${OpenCV_LIB_DIR}) 
link_directories (${RSSDK_DIR}/lib/) 
#link_directories (${RSSDK_DIR}/sample/common/lib/${RSSDK_SUFFIX}/v140)

#
file (GLOB HEADERS *.h)
file (GLOB SOURCES *.cpp
					${SRC_DIR}/TelemetryClient/Telemetry/DBHandler.cpp)

#
set( CMAKE_CXX_FLAGS "/Wall /W4 /Zi /sdl /errorReport:prompt /Oi /nologo /MD -D_CRT_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE")

#
add_library(${PROJECT_NAME} SHARED 
				${HEADERS} ${SOURCES})

#
set_target_properties (${PROJECT_NAME} PROPERTIES 
						VS_DOTNET_REFERENCES "System;System.Data;System.Xml"
						VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.5.2")
#
target_link_libraries(${PROJECT_NAME}
						ConfigurationReader
#						${RSSDK_COMMON_LIBRARY}
						${RSSDK_LIBRARY}
						${Qt5Core_LIBRARIES}
						${Qt5Sql_LIBRARIES}
						${LIBXML2_LIBRARIES}
						${OpenCV_LIBS})
						
add_dependencies (${PROJECT_NAME} ConfigurationReader)
	
install (FILES ${OpenCV_DLL} DESTINATION ${BIN_DIR})
	
install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${BIN_DIR})
