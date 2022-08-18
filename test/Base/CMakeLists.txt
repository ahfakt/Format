cmake_minimum_required(VERSION 3.14.0)
project(StreamFormatTest_Base VERSION 0.1 DESCRIPTION "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE})

set(INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}_Base_00)
target_include_directories(${PROJECT_NAME}_Base_00 PRIVATE include ${UTIL_PATH})
target_link_libraries(${PROJECT_NAME}_Base_00 PRIVATE StreamFormat)
target_sources(${PROJECT_NAME}_Base_00
		PRIVATE
		${SRC_ROOT}/Base_00.cpp)
add_test(NAME ${PROJECT_NAME}_Base_00 COMMAND ${PROJECT_NAME}_Base_00)