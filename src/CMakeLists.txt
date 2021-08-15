cmake_minimum_required(VERSION 3.0)

set(CHIP8_HEADERS ${PROJECT_SOURCE_DIR}/include)

set(CHIP8_SOURCES
    chip8.c ${CHIP8_HEADERS}/chip8.h)

add_executable(chip8 ${CHIP8_SOURCES})
target_include_directories(chip8 PUBLIC ${CHIP8_HEADERS} ${RAYLIB_INCLUDE_DIR})
target_link_libraries(chip8 raylib dl GL m pthread rt X11)

add_library(chip8_lib STATIC ${CHIP8_SOURCES})
target_compile_options(chip8_lib PUBLIC "-DBUILD_STATIC_LIB")
target_include_directories(chip8_lib PUBLIC ${CHIP8_HEADERS})