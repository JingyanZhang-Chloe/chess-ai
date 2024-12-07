cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)

project(blunder-ai)

add_subdirectory(lib)
include_directories(${PROJECT_SOURCE_DIR}/lib/include)

file(GLOB sources exe/*.cpp)

foreach(source IN LISTS sources)
	cmake_path(GET source STEM exec-name)

	add_executable(${exec-name} ${source})
	target_link_libraries(${exec-name} engine)
endforeach()
