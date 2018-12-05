CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 11)

add_library(storager storager/storager.cpp)

add_executable(test_reflector test/test_reflector.cpp storager/storager.cpp)