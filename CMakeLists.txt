cmake_minimum_required(VERSION 3.0)
project(wordle-search CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_NAME ${PROJECT_NAME})

file(GLOB SOURCES src/main.cc src/*.cpp)
add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
