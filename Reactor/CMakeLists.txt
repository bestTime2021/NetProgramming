cmake_minimum_required(VERSION 3.10)

project(Reactor)

if(CMAKE_PROJECT_NAME STREQUAL "Reactor")
	option(MY_PROJECT "build Reactor" ON)
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(net/)

