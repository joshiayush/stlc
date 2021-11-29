cmake_minimum_required(VERSION 3.16.3)

project(cjson)

set(CJSON_INCLUDE_DIRS "../include")

set(CJSON_SSTREAM_ROOT "../cjson/data/sstream")

set(CJSON_SSTREAM_FILES 
    ${CJSON_SSTREAM_ROOT}/sstream.c
    ${CJSON_SSTREAM_ROOT}/fileio.c
    ${CJSON_SSTREAM_ROOT}/iterators.c
    ${CJSON_SSTREAM_ROOT}/modifiers.c)

find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS} ${CJSON_INCLUDE_DIRS})

add_executable(tests tests.cc ${CJSON_SSTREAM_FILES})
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)