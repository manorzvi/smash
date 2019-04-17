# cmake_minimum_required(VERSION <specify CMake version here>)
project(hw1v3)

set(CMAKE_CXX_STANDARD 14)

add_executable(hw1v3 smash.cpp commands.cpp commands.h signals.cpp signals.h Job.cpp Job.h)