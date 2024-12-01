cmake_minimum_required(VERSION 3.29)
project(JsonParse)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DPARSING_DEBUG")
set(ProjectDir .)

set(CMAKE_CXX_STANDARD 20)

add_executable(JsonParse main.cpp)

add_library(json_parser INTERFACE)
target_include_directories(json_parser INTERFACE
        "${ProjectDir}/json-parser"
)

