cmake_minimum_required(VERSION 3.0)

project(chip8_emu)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules/")

option(ENABLE_UT "Perform unit tests after build" OFF)
option(ENABLE_COVERAGE "Build coverage report" ON)

# Coverage
if(ENABLE_COVERAGE)
    find_program(PATH_GCOV NAMES gcov)
    if(NOT PATH_GCOV)
        message(WARNING "gcov not found")
        set(ENABLE_COVERAGE OFF)
    endif()

    find_program(PATH_LCOV NAMES lcov)
    if(NOT PATH_LCOV)
        message(WARNING "lcov not found")
        set(ENABLE_COVERAGE OFF)
    endif()

    find_program(PATH_GENHTML NAMES genhtml)
    if(NOT PATH_GENHTML)
        message(WARNING "genhtml not found")
        set(ENABLE_COVERAGE OFF)
    endif()

    if(ENABLE_COVERAGE)
        set(CMAKE_C_FLAGS_COVERAGE "--coverage -fprofile-arcs -ftest-coverage")
    endif()
endif()

set(CMAKE_C_FLAGS "-Wall -Werror -pedantic ${CMAKE_C_FLAGS_COVERAGE}")

find_package(raylib REQUIRED)

# Building actual project
add_subdirectory(src)

# Unit Tests
if(ENABLE_UT)
    find_package(CMocka)
    if(CMOCKA_FOUND)
        enable_testing()
        add_subdirectory(ut)
    endif(CMOCKA_FOUND)
endif(ENABLE_UT)
