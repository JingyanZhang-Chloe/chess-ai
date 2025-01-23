cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)
string(APPEND CMAKE_CXX_FLAGS " -O3")

file(GLOB_RECURSE sources *.cpp)

add_library(engine ${sources})
target_include_directories(engine PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
