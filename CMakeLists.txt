cmake_minimum_required(VERSION 3.20.0)
project(Format VERSION 0.1 DESCRIPTION "Format related operations on streams")


set(CMAKE_CXX_STANDARD 20)


file(GLOB INC ${CMAKE_CURRENT_SOURCE_DIR}/inc/${PROJECT_NAME}/*.hpp)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} OBJECT)
target_sources(${PROJECT_NAME} PUBLIC ${INC} PRIVATE ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC inc)


set(DEPENDENCIES Stream)
foreach(T IN LISTS DEPENDENCIES)
	if (NOT TARGET ${T})
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../${T} ${CMAKE_CURRENT_BINARY_DIR}/../${T})
	endif ()
endforeach()
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEPENDENCIES})


if (CMAKE_BUILD_TYPE MATCHES Debug)
	enable_testing()
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test ${CMAKE_CURRENT_BINARY_DIR}/Testing)
elseif (CMAKE_BUILD_TYPE MATCHES Release)
	find_package(Doxygen)
	if (DOXYGEN_FOUND)
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/doxygen.cfg ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg @ONLY)
		add_custom_target(${PROJECT_NAME}Doc
			WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/doc
			COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg
			COMMENT "Generating ${PROJECT_NAME} documentation with Doxygen"
			VERBATIM)
	endif (DOXYGEN_FOUND)
endif()