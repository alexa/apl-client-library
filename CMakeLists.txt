cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(APLClientLibrary VERSION 1.0.0 LANGUAGES CXX)

include(BuildOptions.cmake)
#Using APLCore's google test library for standalone build.
if(STANDALONE)
    include(tools/Testing.cmake)
endif()



# Alexa Client SDK targets.
add_subdirectory("APLClient")

if(SANDBOX)
    add_subdirectory("APLClientSandbox")
endif()
