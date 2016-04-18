cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")
set(SOURCE_EXE generate_similarity.cpp)

add_library(trie STATIC trie.cpp)
add_library(document STATIC document.cpp)

add_executable(gen ${SOURCE_EXE})

target_link_libraries(document trie)
target_link_libraries(gen document)
