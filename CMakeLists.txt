cmake_minimum_required(VERSION 2.6)
project(livelock_sim)

add_compile_options(-std=c++11)
set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0037 NEW)

find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})

link_directories(${BOOST_LIBRARY_DIR})

add_library(${PROJECT_NAME}_lib
	src/timer.cpp
	src/input_output.cpp
	src/random.cpp
	src/processor.cpp
	src/param_reader.cpp
	)

add_executable(livelock_sim src/livelock_sim.cpp)


if(UNIX)
target_link_libraries(
	livelock_sim 
		${PROJECT_NAME}_lib
		Boost::program_options
	)
elseif(APPLE)
	target_link_libraries(
		livelock_sim 
			${PROJECT_NAME}_lib
			-lboost_program_options
		)
endif()

