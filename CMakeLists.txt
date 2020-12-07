
cmake_minimum_required(VERSION 3.13)
project(rpg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(rpg "main.cpp")

add_subdirectory(cpp)
add_subdirectory(headers)
target_include_directories(rpg PRIVATE headers/)
