cmake_minimum_required(VERSION 3.29)
project(JsonParse)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DPARSING_DEBUG")
set(ProjectDir .)

set(CMAKE_CXX_STANDARD 20)

add_executable(JsonParse main.cpp
        data-schema.h)

file(GLOB JSON_PARSER_SOURCES "../json-parser/*.c" "../json-parser/*.h")
add_library(json_parser INTERFACE ${JSON_PARSER_SOURCES} ../json-parser/exceptions
        ../json-parser/schema-value.h
        ../json-parser/schema-factory.h
        ../json-parser/instance_handler.h
        ../json-parser/parser.h)

target_include_directories(json_parser INTERFACE
        "${ProjectDir}/json-parser"
)