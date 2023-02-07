cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(INCLUDE_PATH "${APLClient_SOURCE_DIR}/include/")

if(POLICY CMP0057)
    cmake_policy(SET CMP0057 NEW)
endif()

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

include(CTest)

add_custom_target(unit ALL COMMAND ${CMAKE_CTEST_COMMAND})

macro(discover_unit_tests includes libraries)
    # This will result in some errors not finding GTest when running cmake, but allows us to better integrate with CTest
    # TODO: update to use gtest_discover_tests
    find_package(GTest ${GTEST_PACKAGE_CONFIG})

    add_definitions("-DGTEST_LINKED_AS_SHARED_LIBRARY=1")
    set (extra_macro_args ${ARGN})
    LIST(LENGTH extra_macro_args num_extra_args)
    if (${num_extra_args} GREATER 0)
        list(GET extra_macro_args 0 inputs)
    endif()
    file(GLOB_RECURSE tests RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*Test.cpp")
    foreach(testsourcefile IN LISTS tests)
        get_filename_component(testname ${testsourcefile} NAME_WE)
        add_executable(${testname} ${testsourcefile})
        add_dependencies(unit ${testname})
        target_include_directories(${testname} PRIVATE ${includes})
        # Do not include gtest_main due to double free issue
        # - https://github.com/google/googletest/issues/930
        target_link_libraries(${testname} ${libraries} gmock_main)

        configure_test_command(${testname} "${inputs}" ${testsourcefile})
    endforeach()
endmacro()

macro(configure_test_command testname inputs testsourcefile)
        GTEST_ADD_TESTS(${testname} "${inputs}" ${testsourcefile})
endmacro()

discover_unit_tests("${INCLUDE_PATH}" "APLClient")
