#
if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)
	cmake_minimum_required (VERSION 2.8.11)
else()
	cmake_minimum_required (VERSION 2.8.5)
endif ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)

#
project (DexProcessingUnit)

#
#include (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)

#
include_directories (.)

if (LIBXML2_INCLUDE_DIR)
	include_directories (${LIBXML2_INCLUDE_DIR})
else ()
	message (SEND_ERROR "LIBXML2_INCLUDE_DIR not set in config.cmake")
endif (LIBXML2_INCLUDE_DIR)

if (WINDOWS OR WINRT)
	if (LIBICONV_INCLUDE_DIR)
		include_directories (${LIBICONV_INCLUDE_DIR})
	else ()
		message (SEND_ERROR "LIBICONV_INCLUDE_DIR not set in config.cmake")
	endif (LIBICONV_INCLUDE_DIR)
endif (WINDOWS OR WINRT)

#
file (GLOB HEADERS *.h)
file (GLOB SOURCES *.cpp)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	# disable 4127 warning
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd\"4127\" /Wall /GL /Gy /W4 /Zi /sdl /errorReport:prompt /Oi /EHsc /nologo")
	set (CMAKE_SHARED_LINKER_FLAGS "/LTCG:incremental")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -std=c++11")
endif ()

#
add_library(${PROJECT_NAME} SHARED 
			${HEADERS} ${SOURCES})

if (NOT LIBXML2_LIBRARIES)
	message (SEND_ERROR "LIBXML2_LIBRARIES not set in config.cmake")
endif (NOT LIBXML2_LIBRARIES)			

#
if (WINDOWS OR WINRT)
	add_library (LibXML SHARED IMPORTED)
	set_property (TARGET LibXML 
					PROPERTY IMPORTED_IMPLIB ${LIBXML2_LIBRARIES})
	
	if (Qt5Core_FOUND)
		#Link with Qt5 
		target_link_libraries (${PROJECT_NAME} 
								LibXML
								Qt5::Core 
								Qt5::Sql)
	elseif (QT4_FOUND)
		target_link_libraries (${PROJECT_NAME} 
								LibXML
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
								${LIBXML2_LIBRARIES}
								Qt5::Core 
								Qt5::Sql)
	elseif (QT4_FOUND)
		target_link_libraries (${PROJECT_NAME} 
								${LIBXML2_LIBRARIES}
								${QT_LIBRARIES})
	endif (Qt5Core_FOUND)
	
	install (TARGETS ${PROJECT_NAME} 
				LIBRARY DESTINATION ${BIN_DIR})
endif (UNIX)


