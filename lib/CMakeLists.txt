cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE sources *.cpp)

add_library(engine ${sources})
