cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")
set(SOURCE_EXE cluster.cpp)

include_directories(libs ../lib/)
add_subdirectory(../lib/ lib)

add_executable(cluster ${SOURCE_EXE})

target_link_libraries(cluster document)
