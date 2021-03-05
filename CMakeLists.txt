cmake_minimum_required(VERSION 3.15)

if (POLICY CMP0092)
    cmake_policy(SET CMP0092 NEW)
endif ()

project(tuatara)

set(CMAKE_CXX_STANDARD 17)

find_package(BISON 3.6.1 REQUIRED)
find_package(FLEX 2.6 REQUIRED)
find_package(Python3 REQUIRED)

bison_target(
        PARSER ${CMAKE_SOURCE_DIR}/src/parser.y
        ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc
        VERBOSE
        COMPILE_FLAGS "-Wall --report=all --xml"
)
flex_target(
        SCANNER ${CMAKE_SOURCE_DIR}/src/scanner.l
        ${CMAKE_CURRENT_BINARY_DIR}/scanner.yy.cc
)
add_flex_bison_dependency(SCANNER PARSER)

include_directories(${CMAKE_CURRENT_BINARY_DIR} include)

file(GLOB_RECURSE SRC "src/*")

add_executable(${PROJECT_NAME}
        ${BISON_PARSER_OUTPUTS}
        ${FLEX_SCANNER_OUTPUTS}
        ${SRC})

set(CMAKE_CONFIGURATION_TYPES Debug Analysis Coverage Release CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_ANALYSIS "" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "" CACHE STRING "" FORCE)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/grammar.h
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/src/parse_grammar.py ${CMAKE_CURRENT_BINARY_DIR}/parser.xml ${CMAKE_CURRENT_BINARY_DIR}/grammar.h
        MAIN_DEPENDENCY ${CMAKE_CURRENT_BINARY_DIR}/parser.output
        DEPENDS ${BISON_PARSER_OUTPUTS} ${CMAKE_SOURCE_DIR}/src/parse_grammar.py
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/grammar.h
        COMMENT "Generating grammar"
)


# Tests
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
        )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
        )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
        "${CMAKE_BINARY_DIR}/googletest-build"
        )

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
            "${gmock_SOURCE_DIR}/include"
            )
endif ()
include(GoogleTest)
set(TESTING_NAME "all_tests")
file(GLOB_RECURSE TEST_SRC "test/*" "src/*")
list(REMOVE_ITEM TEST_SRC "${CMAKE_SOURCE_DIR}/src/main.cpp")
enable_testing()
add_executable(
        ${TESTING_NAME}
        ${BISON_PARSER_OUTPUTS}
        ${FLEX_SCANNER_OUTPUTS}
        ${TEST_SRC}
)
# Link against gtest
target_link_libraries(${TESTING_NAME} gtest_main)
gtest_discover_tests(${TESTING_NAME})

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Zc:__cplusplus /W4)
    target_compile_options(${TESTING_NAME} PRIVATE /Zc:__cplusplus /W4)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
    if (CMAKE_BUILD_TYPE MATCHES Analysis)
        target_compile_options(${PROJECT_NAME} PRIVATE /analyze)
        target_compile_options(${TESTING_NAME} PRIVATE /analyze)
    endif ()
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wshadow
            -pedantic)
    target_compile_options(${TESTING_NAME} PRIVATE -O0 -Wall -Wextra -Wshadow
            -pedantic)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(-Wsuggest-override HAVE_WSUGGEST_OVERRIDE)
    if (HAVE_WSUGGEST_OVERRIDE)
        target_compile_options(${PROJECT_NAME} PRIVATE -Wsuggest-override)
        target_compile_options(${TESTING_NAME} PRIVATE -Wsuggest-override)
    endif ()
    if (CMAKE_BUILD_TYPE MATCHES Analysis)
        check_cxx_compiler_flag(-fanalyzer HAVE_FANALYZE)
        if (HAVE_FANALYZE)
            target_compile_options(${PROJECT_NAME} PRIVATE -fanalyzer)
            target_compile_options(${TESTING_NAME} PRIVATE -fanalyzer)
        endif ()
        check_cxx_compiler_flag(-analyze HAVE_ANALYZE)
        if (HAVE_ANALYZE)
            target_compile_options(${PROJECT_NAME} PRIVATE --analyze)
            target_compile_options(${TESTING_NAME} PRIVATE --analyze)
        endif ()
    endif ()
    if (CMAKE_BUILD_TYPE MATCHES Coverage)
        target_compile_options(${PROJECT_NAME} PRIVATE -fprofile-arcs -ftest-coverage)
        target_compile_options(${TESTING_NAME} PRIVATE -fprofile-arcs -ftest-coverage)
        if (APPLE)
            target_link_libraries(${PROJECT_NAME} --coverage)
            target_link_libraries(${TESTING_NAME} --coverage)
        else ()
            target_link_libraries(${PROJECT_NAME} -lgcov --coverage)
            target_link_libraries(${TESTING_NAME} -lgcov --coverage)
        endif ()
    endif ()
endif ()
