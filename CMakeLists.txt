cmake_minimum_required(VERSION 2.6)
project(livelock_sim)

add_compile_options(-std=c++11)
set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0037 NEW)

include_directories(
	include
	)

add_library(${PROJECT_NAME}_lib
	src/timer.cpp
	src/input_output.cpp
	src/random.cpp
	src/processor.cpp
	)

add_executable(livelock_sim src/livelock_sim.cpp)

target_link_libraries(
	livelock_sim ${PROJECT_NAME}_lib
	)