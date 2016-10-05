cmake_minimum_required(VERSION 3.6)
project(sandbox-vision-opencv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(opencv ${SOURCE_FILES})