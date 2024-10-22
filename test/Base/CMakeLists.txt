cmake_minimum_required(VERSION 3.20.0)
project(${PROJECT_NAME}_${Class} VERSION 0.1 DESCRIPTION "")


set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}_Base_00)
target_sources(${PROJECT_NAME}_Base_00 PRIVATE ${SRC_ROOT}/Base_00.cpp)
target_link_libraries(${PROJECT_NAME}_Base_00 PRIVATE Stream Stream_Test Format)
add_test(NAME ${PROJECT_NAME}_Base_00 COMMAND ${PROJECT_NAME}_Base_00)