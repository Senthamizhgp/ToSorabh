# Find OS for which CMake is to build
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set (WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set (UNIX TRUE)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore")
	set (WINRT TRUE)
endif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

# Enable verbose output from Makefile builds
set(CMAKE_VERBOSE_MAKEFILE TRUE)

# Following variables used by CMake for building libraries 
# and Telemetry application
################################################################
# PROJ_DIR	: Two directory up from where config.cmake is found 
#			  and this variable is used to reference the following variables.
# SRC_DIR	: Path where source present.
# BUILD_DIR	: Directory where the build files are placed.
# BIN_DIR	: Directory where final bin and libraries are placed.
################################################################
set (PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set (SRC_DIR ${PROJ_DIR}/Source_Code)
set (BUILD_DIR ${PROJ_DIR}/Build)
if (WINDOWS OR WINRT)
	set (BIN_DIR ${PROJ_DIR}/Bin/Windows)
elseif (UNIX)
	set (BIN_DIR ${PROJ_DIR}/bin)
endif (WINDOWS OR WINRT)

if (WINDOWS OR WINRT)
	set (CMAKE_CONFIGURATION_TYPES "Release;Debug")
	
	# Set system environment variable CMAKE_PREFIX_PATH to <Qt installation path >
	# as mentioned in CMake user guide
	################################################################
	#set (CMAKE_PREFIX_PATH <Qt_Dir>)
	
	# Set the following variables for LibXml2 library
	################################################################
	# LIBXML2_INCLUDE_DIR	: Directory where LibXml2 headers are found.
	# LIBXML2_LIBRARIES		: Full path (inluding the library name) 
	#						  where libxml2.lib is found.
	# LIBXML2_DLL			: Full path (inluding the library name) 
	#						  where libxml2.dll is found.
	################################################################
	set (LIBXML2_INCLUDE_DIR ${PROJ_DIR}/Lib/LibXML/Include)
	set (LIBXML2_LIBRARIES ${PROJ_DIR}/Lib/LibXML/Lib/libxml2.lib)
	set (LIBXML2_DLL ${PROJ_DIR}/Lib/LibXML/Lib/libxml2.dll)
	
	# Set the following variables for libiconv header
	################################################################
	# LIBICONV_INCLUDE_DIR	: Directory where Libiconv headers are found.
	#						  (libxml2 require these headers)
	################################################################
	set (LIBICONV_INCLUDE_DIR ${PROJ_DIR}/Lib/Libiconv/Include)

	# Set the following variables for MQTT library
	################################################################
	# MQTT_LIBRARIES	: Full path (inluding the library name) 
	#				      where paho-mqtt3as.lib is found.
	# MQTT_DLL			: Full path (inluding the library name) 
	#				      where paho-mqtt3as.dll is found.
	################################################################
	set (MQTT_LIBRARIES ${PROJ_DIR}/Lib/PahoMQTT/Lib/paho-mqtt3as.lib)
	set (MQTT_DLL ${PROJ_DIR}/Lib/PahoMQTT/Lib/paho-mqtt3as.dll)
	
	# The following variables is for Sunset Pass module
	# ONLY MODIFY IF SUNSET PASS LIBRARIES MOVED TO DIFFERENT DIRECTORY
	set (HLAPI_LIBRARIES ${PROJ_DIR}/Lib/HLAPI/Lib/HLAPI.lib)
	set (LLAPI_LIBRARIES ${PROJ_DIR}/Lib/LLAPI/Lib/LLAPI.lib)
	set (HLAPI_DLL ${PROJ_DIR}/Lib/HLAPI/Lib/HLAPI.dll)
	set (LLAPI_DLL ${PROJ_DIR}/Lib/LLAPI/Lib/LLAPI.dll)
	
	# Set OPEN_SSL_DLL with ssl library files. We use 
	# GLOB option to select all dll from directory where it is installed.
	FILE (GLOB OPEN_SSL_DLL
			${PROJ_DIR}/Lib/OpenSSL/Lib/*.dll)
	
	# Following variables are used to set RealSense SDK directories
	# used for building RealSense People Counter module.

	set(PF32 "ProgramFiles(x86)")
	set(PF64 "ProgramW6432")

	message ("PF32 = $ENV{${PF32}}")
	message ("PF64 = $ENV{${PF64}}")

	find_path (RSSDK_DIR include/pxcversion.h
				 PATHS "$ENV{RSSDK_DIR}" 
						"$ENV{${PF86}}/Intel/RSSDK" 
						"$ENV{${PF64}}/RSSDK" 
				 DOC "RealSense SDK directory")
				
	if (RSSDK_DIR)
		message("RealSense SDK found in ${RSSDK_DIR}")
		set (RSSDK_INCLUDE_DIRS ${RSSDK_DIR}/include)
		mark_as_advanced (RSSDK_INCLUDE_DIRS)
		
		#set (RSSDK_COMMON_INCLUDE_DIRS ${RSSDK_DIR}/sample/common/include)
		#mark_as_advanced (RSSDK_COMMON_INCLUDE_DIRS)
		
		# Libraries
		set (RSSDK_RELEASE_NAME libpxc.lib)
		#set (RSSDK_COMMON_RELEASE_NAME libpxcutils.lib)
		set (RSSDK_SUFFIX Win32)

		find_library (RSSDK_LIBRARY
					   NAMES ${RSSDK_RELEASE_NAME}
					   PATHS "${RSSDK_DIR}/lib/" NO_DEFAULT_PATH
					   PATH_SUFFIXES ${RSSDK_SUFFIX})
		mark_as_advanced (RSSDK_LIBRARY)			   
				   
#		find_library (RSSDK_COMMON_LIBRARY
#					   NAMES ${RSSDK_COMMON_RELEASE_NAME}
#					   PATHS "${RSSDK_DIR}/sample/common/lib/${RSSDK_SUFFIX}/v140" NO_DEFAULT_PATH
#					   PATH_SUFFIXES ${RSSDK_SUFFIX})

		if (RSSDK_LIBRARY_NOTFOUND)
			message("RSSDK_LIBRARY not found ")
		endif(RSSDK_LIBRARY_NOTFOUND)

#		if (RSSDK_COMMON_LIBRARY_NOTFOUND)
#			message("RSSDK_COMMON_LIBRARY not found ")
#		endif(RSSDK_COMMON_LIBRARY_NOTFOUND)

		set (RSSDK_LIBRARIES optimized ${RSSDK_LIBRARY})
		#mark_as_advanced (RSSDK_COMMON_LIBRARY)
		
		#set (OpenCV_DIR "C:\\opencv\\build")
		find_package (OpenCV REQUIRED)
		if (NOT OpenCV_FOUND)
			message (FATAL_ERROR "OpenCV not found")
		else()
			message("OpenCV found in ${OpenCV-DIR}")
		endif (NOT OpenCV_FOUND)

		message (${OpenCV_LIBS})
		file (GLOB OpenCV_DLL
				${OpenCV_LIB_PATH}/../bin/opencv_core2413.dll
				${OpenCV_LIB_PATH}/../bin/opencv_highgui2413.dll
				${OpenCV_LIB_PATH}/../bin/opencv_imgproc2413.dll
				${OpenCV_LIB_PATH}/../bin/opencv_objdetect2413.dll)

	endif (RSSDK_DIR)
elseif (UNIX)
	# Do not add run time path information
	set (CMAKE_SKIP_RPATH TRUE)
	
	find_package (LibXml2 QUIET REQUIRED)
	if (NOT ${LIBXML2_FOUND})
		message(SEND_ERROR "Can not find LibXml2")
	else (${LIBXML2_FOUND})
		find_path (LIBXML2_INCLUDE_DIR libxml/xpath.h)
	endif (NOT ${LIBXML2_FOUND})
	
	# Set the following variables for MQTT library
	################################################################
	# MQTT_LIBRARIES	: Full path (inluding the library name) 
	#				      where libpaho-mqtt3as.so is found.
	################################################################	
	set (MQTT_LIBRARIES ${PROJ_DIR}/Lib/PahoMQTT/Lib/libpaho-mqtt3as.so)
	
	set (HLAPI_LIBRARIES ${PROJ_DIR}/Lib/HLAPI/Lib/libHLAPI.so)
	set (LLAPI_LIBRARIES ${PROJ_DIR}/Lib/LLAPI/Lib/libLLAPI.so)
endif (WINDOWS OR WINRT)

set (MQTT_INCUDE_DIR ${PROJ_DIR}/Lib/PahoMQTT/Include)

find_package (Qt5Core QUIET)
if (NOT Qt5Core_FOUND)
	#find qt4 package
	find_package (Qt4 REQUIRED)
	if (NOT QT4_FOUND)
		message( SEND_ERROR "Can not find Qt")
	endif (NOT QT4_FOUN)
	#include Qt files
	include (${QT_USE_FILE})
else ()
	find_package (Qt5Sql REQUIRED)
endif (NOT Qt5Core_FOUND)