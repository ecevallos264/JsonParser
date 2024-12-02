cmake_minimum_required(VERSION 3.29)
project(JsonParse)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DPARSING_DEBUG")
set(ProjectDir .)

set(CMAKE_CXX_STANDARD 20)

add_executable(JsonParse main.cpp
        data-schema.h)

add_library(json_parser INTERFACE
        ../json-parser/optional.h
        ../json-parser/translater.h
        ../json-parser/schema_handler.h)
target_include_directories(json_parser INTERFACE
        "${ProjectDir}/json-parser"
)

