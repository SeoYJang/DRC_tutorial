cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(dual-readout)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(rootIO)
add_subdirectory(DRsim)
add_subdirectory(analysis)
