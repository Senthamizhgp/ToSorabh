# Cmake minimum required version#
if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)
	cmake_minimum_required (VERSION 2.8.11)
else ()
	cmake_minimum_required (VERSION 2.8.5)
endif ("${CMAKE_VERSION}" VERSION_GREATER 2.8.5)

# Maps to a solution file ConfigurationReaderf.sln
project (ConfigurationReader)

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

#
if (WINDOWS OR WINRT)
	set(CMAKE_CXX_FLAGS "/Wall /GL /Gy /W4 /Zi /sdl /errorReport:prompt /Oi /EHsc /nologo")
	set(CMAKE_SHARED_LINKER_FLAGS "/LTCG:incremental")
elseif (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -D_GLIBCXX_USE_NANOSLEEP -std=gnu++0x -pthread")
endif (WINDOWS OR WINRT)

#
add_library (${PROJECT_NAME} SHARED 
				${HEADERS} ${SOURCES})

if (NOT LIBXML2_LIBRARIES)
	message (SEND_ERROR "LIBXML2_LIBRARIES not set in config.cmake")
endif (NOT LIBXML2_LIBRARIES)

#
if (WINDOWS OR WINRT)
	add_library (LibXML SHARED IMPORTED)
	set_property (TARGET LibXML 
					PROPERTY IMPORTED_IMPLIB ${LIBXML2_LIBRARIES})
				
	#link all dependencies to target
	target_link_libraries (${PROJECT_NAME} 
							LibXML)

	#install target library to given path
	install (TARGETS ${PROJECT_NAME}
				RUNTIME DESTINATION ${BIN_DIR})
endif (WINDOWS OR WINRT)

if (UNIX)
	#link all dependencies to target
	target_link_libraries (${PROJECT_NAME}
						   ${LIBXML2_LIBRARIES})
	
	#install target library to given path
	install (TARGETS ${PROJECT_NAME} 
				LIBRARY DESTINATION ${BIN_DIR})
endif(UNIX)