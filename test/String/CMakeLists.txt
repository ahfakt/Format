cmake_minimum_required(VERSION 3.14.0)
project(StreamFormatTestString VERSION 0.1 DESCRIPTION "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE})

set(INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}_Integer)
target_include_directories(${PROJECT_NAME}_Integer PRIVATE include)
target_link_libraries(${PROJECT_NAME}_Integer PRIVATE StreamFormat)
target_sources(${PROJECT_NAME}_Integer
		PRIVATE
		${SRC_ROOT}/Integer.cpp)
add_test(NAME ${PROJECT_NAME}_Integer COMMAND ${PROJECT_NAME}_Integer)

add_executable(${PROJECT_NAME}_Float)
target_include_directories(${PROJECT_NAME}_Float PRIVATE include)
target_link_libraries(${PROJECT_NAME}_Float PRIVATE StreamFormat)
target_sources(${PROJECT_NAME}_Float
		PRIVATE
		${SRC_ROOT}/Float.cpp)
add_test(NAME ${PROJECT_NAME}_Float COMMAND ${PROJECT_NAME}_Float)