cmake_minimum_required (VERSION 2.8)
project (rewiringPi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_subdirectory (rewiringPi)
add_subdirectory (tests)
