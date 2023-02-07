cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(APLClientLibrary VERSION 1.0.0 LANGUAGES CXX)

include(BuildOptions.cmake)

add_subdirectory("thirdparty")

# Alexa Client SDK targets.
add_subdirectory("APLClient")

if(SANDBOX)
    add_subdirectory("APLClientSandbox")
endif()
