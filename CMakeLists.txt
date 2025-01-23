cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Release)
string(APPEND CMAKE_CXX_FLAGS "-O3")

project(blunder-ai)

add_subdirectory(lib)
include_directories(${PROJECT_SOURCE_DIR}/lib/include)

file(GLOB sources exe/*.cpp)

# Build executables
foreach(source IN LISTS sources)
	cmake_path(GET source STEM exec-name)

	add_executable(${exec-name} ${source})
	target_link_libraries(${exec-name} PRIVATE engine)
endforeach()

file(GLOB tests tests/*.cpp)

# Build unit tests
foreach(test IN LISTS tests)
	cmake_path(GET test STEM test-name)

	add_executable("TEST_${test-name}" ${test})
	target_link_libraries("TEST_${test-name}" PRIVATE engine)
endforeach()
