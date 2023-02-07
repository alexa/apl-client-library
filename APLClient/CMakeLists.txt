cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(APLClient LANGUAGES CXX)

add_subdirectory("src")

if (BUILD_UNIT_TESTS)
    add_subdirectory("test")
endif()
