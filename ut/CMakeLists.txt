set(CHIP8_HEADERS_DIR ${PROJECT_SOURCE_DIR}/include)
set(CHIP8_SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)
set(TEST_SOURCES_DIR ${PROJECT_SOURCE_DIR}/ut)

set(tests cycle_0_7 cycle_8 cycle_9_D cycle_E cycle_F basic)

foreach(test_name IN LISTS tests)
    add_executable(${test_name} ${test_name}.c 
        ${TEST_SOURCES_DIR}/common.c
        ${TEST_SOURCES_DIR}/common.h
        ${CHIP8_HEADERS_DIR}/chip8.h 
        ${CHIP8_SOURCES_DIR}/chip8.c)
    target_include_directories(${test_name} PRIVATE ${CMOCKA_INCLUDE_DIR})
    target_include_directories(${test_name} PRIVATE ${CHIP8_HEADERS_DIR})
    target_include_directories(${test_name} PRIVATE ${CHIP8_SOURCES_DIR})
    target_include_directories(${test_name} PRIVATE ${TEST_SOURCES_DIR})
    target_link_libraries(${test_name} cmocka chip8_lib)
    add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
endforeach()

if(ENABLE_COVERAGE)
    # Destination
    set(COVERAGE_DIR        "${CMAKE_BINARY_DIR}/ut/code_coverage/")
    set(COVERAGE_FILE_RAW   "${CMAKE_BINARY_DIR}/ut/coverage_raw.info")
    set(COVERAGE_FILE_CLEAN "${CMAKE_BINARY_DIR}/ut/coverage_clean.info")

    # Add coverage target
    add_custom_target(coverage
        COMMENT "Generating code coverage..."
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        # Cleanup code counters
        COMMAND "${PATH_LCOV}" --directory . --zerocounters --quiet
        # Run tests
        COMMAND "${CMAKE_CTEST_COMMAND}" --quiet
        # Capture the counters
        COMMAND "${PATH_LCOV}"
            --directory .
            --rc lcov_branch_coverage=1
            --rc 'lcov_excl_line=assert'
            --capture --quiet
            --output-file "${COVERAGE_FILE_RAW}"
        # Remove coverage of tests, system headers, etc.
        COMMAND "${PATH_LCOV}"
            --remove "${COVERAGE_FILE_RAW}" '${CMAKE_SOURCE_DIR}/ut/*'
            --rc lcov_branch_coverage=1
            --quiet --output-file "${COVERAGE_FILE_CLEAN}"
        # Generate HTML report
        COMMAND "${PATH_GENHTML}"
            --branch-coverage --function-coverage --quiet --title "chip8 emulator"
            --legend --show-details --output-directory "${COVERAGE_DIR}"
            "${COVERAGE_FILE_CLEAN}"
        # Delete the counters
        COMMAND "${CMAKE_COMMAND}" -E remove
            ${COVERAGE_FILE_RAW} ${COVERAGE_FILE_CLEAN}
        )

    add_custom_command(TARGET coverage POST_BUILD
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/ut"
        COMMENT "To see the code coverage report, open ${COVERAGE_DIR}index.html"
        COMMAND ;
        )
endif()
