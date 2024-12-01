cmake_minimum_required(VERSION 3.5)

project(chess_ai)

file(GLOB sources "src/*.cpp")
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${sources})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

