cmake_minimum_required(VERSION 3.11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc-7")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-7")

set(CMAKE_CXX_FLAGS "-O2 -std=c++11 -Wall")

project(Sudoku_ter)

add_subdirectory(src)

include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(Sudoku main.cpp)

target_link_libraries(Sudoku source_lib stdc++)
