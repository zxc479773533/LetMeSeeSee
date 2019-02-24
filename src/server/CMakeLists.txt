CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(server)

include_directories(/usr/include/LetMeSeeSee)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fPIC)
set(CMAKE_INSTALL_PREFIX /usr)

file(GLOB NET_SOURCE net/*.cpp)
add_library(net ${NET_SOURCE})
file(GLOB STORAGER_SOURCE storager/*.cpp)
add_library(storager SHARED ${STORAGER_SOURCE})
set_target_properties(storager PROPERTIES VERSION 1.0 SOVERSION 1)
add_library(storager_static ${STORAGER_SOURCE})
find_package(Threads)
target_link_libraries(storager net ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(storager_static net ${CMAKE_THREAD_LIBS_INIT})

# 安装动态链接库
install(TARGETS storager storager_static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib) 
install(FILES storager/storager.h DESTINATION include/LetMeSeeSee)

# 编译测试文件
add_executable(func_test test/func_test.cpp)
target_link_libraries(func_test storager)
add_executable(net_test test/net_test.cpp)
target_link_libraries(net_test storager)
