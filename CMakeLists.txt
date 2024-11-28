cmake_mimimum_required(VERSION 3.5)

project(chess_ai)

file(GLOB sources "src/*.cpp")

add_executable(${PROJECT_NAME} ${source})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
