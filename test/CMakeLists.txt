cmake_minimum_required(VERSION 3.14.0)
project(StreamFormatTest VERSION 0.1 DESCRIPTION "")

set(UTIL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../Stream/test/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Base ${CMAKE_CURRENT_BINARY_DIR}/Base)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/String ${CMAKE_CURRENT_BINARY_DIR}/String)