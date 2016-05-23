cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")

add_library(trie STATIC trie.cpp)
add_library(config STATIC config.cpp)
add_library(document STATIC document.cpp)

target_link_libraries(document config)
target_link_libraries(document trie)