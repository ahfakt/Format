cmake_minimum_required(VERSION 3.14.0)
project(FormatTest_String VERSION 0.1 DESCRIPTION "")


set(INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}_Integer)
target_link_libraries(${PROJECT_NAME}_Integer PRIVATE Stream Format)
target_sources(${PROJECT_NAME}_Integer PRIVATE ${SRC_ROOT}/Integer.cpp)
add_test(NAME ${PROJECT_NAME}_Integer COMMAND ${PROJECT_NAME}_Integer)

add_executable(${PROJECT_NAME}_Float)
target_link_libraries(${PROJECT_NAME}_Float PRIVATE Stream Format)
target_sources(${PROJECT_NAME}_Float PRIVATE ${SRC_ROOT}/Float.cpp)
add_test(NAME ${PROJECT_NAME}_Float COMMAND ${PROJECT_NAME}_Float)

add_executable(${PROJECT_NAME}_Line)
target_link_libraries(${PROJECT_NAME}_Line PRIVATE Stream Format)
target_sources(${PROJECT_NAME}_Line PRIVATE ${SRC_ROOT}/Line.cpp)
add_test(NAME ${PROJECT_NAME}_Line COMMAND ${PROJECT_NAME}_Line)