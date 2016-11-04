#
cmake_minimum_required(VERSION 3.0.0)

#
set(CMAKE_VERBOSE_MAKEFILE true)

#
add_subdirectory(channel_selector)
add_subdirectory(config_lib)
add_subdirectory(db-connector)
add_subdirectory(hashtable)
add_subdirectory(pkt_capture)
add_subdirectory(pkt_parser)
add_subdirectory(radiotap)
