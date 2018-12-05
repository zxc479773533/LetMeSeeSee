CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 11)

add_library(storer storer/store.cpp)

add_executable(test_reflector test/test_reflector.cpp storer/store.cpp)