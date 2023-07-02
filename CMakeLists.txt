cmake_minimum_required(VERSION 3.20.0)
project(Format VERSION 0.1 DESCRIPTION "Format related operations on streams")


set(DEPENDENCIES Stream)
set(INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_LIST
		${INC_ROOT}/${PROJECT_NAME}/Base.hpp
		${INC_ROOT}/${PROJECT_NAME}/Dot.hpp
		${INC_ROOT}/${PROJECT_NAME}/String.hpp)
set(SRC_LIST
		${SRC_ROOT}/${PROJECT_NAME}/String.tpp
		${SRC_ROOT}/Base.cpp
		${SRC_ROOT}/Dot.cpp
		${SRC_ROOT}/String.cpp)


add_library(${PROJECT_NAME} OBJECT)
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_sources(${PROJECT_NAME} PUBLIC ${INC_LIST} PRIVATE ${SRC_LIST})

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
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxygen.cfg ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg @ONLY)
		add_custom_target(${PROJECT_NAME}Doc
			WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/doc
			COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg
			COMMENT "Generating ${PROJECT_NAME} documentation with Doxygen"
			VERBATIM)
	endif (DOXYGEN_FOUND)
endif()