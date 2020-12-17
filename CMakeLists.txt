cmake_minimum_required(VERSION 3.17)
project(STL_graph_with_unit-tests)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Google_Test)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(STL_graph_with_unit-tests main.cpp Graph.hpp)
target_link_libraries(STL_graph_with_unit-tests gtest gtest_main)

