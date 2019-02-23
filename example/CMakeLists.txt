cmake_minimum_required(VERSION 3.9)

project(example)

# Include the LetMeSeeSee header
include_directories(/usr/include/LetMeSeeSee)

set(CMAKE_CXX_STANDARD 11)

add_executable(RemoteSM src/RemoteSysMonitor.cpp)

# Link the storager lib
target_link_libraries(RemoteSM -lstorager)