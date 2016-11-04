#CMake minimum required version
cmake_minimum_required(VERSION 3.0.0)

#
project(channelselector)

#
set(CMAKE_VERBOSE_MAKEFILE true)

#
include_directories(./include)

#
file(GLOB ALL_HEADERS include/*.h)
file(GLOB ALL_SOURCES src/*.c)

#
set(CMAKE_C_FLAGS " -g -Wall -Wextra -std=c99 -fPIC -Wfloat-equal -Wundef -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wconversion -Wunreachable-code ")

#
add_library(${PROJECT_NAME} STATIC 
			${ALL_SOURCES})
