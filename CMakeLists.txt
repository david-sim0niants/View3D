cmake_minimum_required(VERSION 3.13)
project(View3D CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(ROOT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SHARED_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/share)

option(BUILD_CLI "Build command line interface" ON)

add_subdirectory(third_party)
add_subdirectory(src)
