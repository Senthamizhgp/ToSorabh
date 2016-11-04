#
if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)
	cmake_minimum_required (VERSION 2.8.11)
else()
	cmake_minimum_required (VERSION 2.8.5)
endif ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)

#
project (TimerLib)

#
#include (${CMAKE_CURRENT_SOURCE_DIR}/../config.cmake)

#
file(GLOB HEADERS *.h)
file(GLOB SOURCES *.c)

#
if (WINDOWS)
	set(CMAKE_C_FLAGS "/Wall /Gy /W4 /Zi /sdl /errorReport:prompt /Oi /EHsc /nologo")
	set(CMAKE_SHARED_LINKER_FLAGS "/LTCG:incremental")
elseif (UNIX)
	set(CMAKE_C_FLAGS "-g -Wall -Wextra")
endif (WINDOWS)

#
add_library(${PROJECT_NAME} STATIC 
			${HEADERS} ${SOURCES})

#
target_link_libraries (${PROJECT_NAME})
