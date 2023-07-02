cmake_minimum_required(VERSION 3.20.0)
project(${PROJECT_NAME}_Test VERSION 0.1 DESCRIPTION "")


set(StreamTest ${CMAKE_CURRENT_SOURCE_DIR}/../../Stream/test)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Base ${CMAKE_CURRENT_BINARY_DIR}/Base)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/String ${CMAKE_CURRENT_BINARY_DIR}/String)