cmake_minimum_required(VERSION 3.10)
project(tuatara)

set(CMAKE_CXX_STANDARD 17)

find_package(BISON 3.2)
find_package(FLEX)

bison_target(
        Parser ${CMAKE_SOURCE_DIR}/src/parser.y
        ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
)
flex_target(
        Scanner ${CMAKE_SOURCE_DIR}/src/scanner.l
        ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/scanner.h
)
add_flex_bison_dependency(Scanner Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR} include)

file(GLOB_RECURSE SRC "src/*")

add_executable(${PROJECT_NAME}
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Scanner_OUTPUTS}
        ${SRC})

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
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Scanner_OUTPUTS}
        ${TEST_SRC}
)
# Link against gtest
target_link_libraries(${TESTING_NAME} gtest_main)
gtest_discover_tests(${TESTING_NAME})

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /analyze)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic
            -Wno-unused-parameter -fprofile-arcs -ftest-coverage)
    target_compile_options(${TESTING_NAME} PRIVATE -O0 -Wall -Wextra -pedantic
            -Wno-unused-parameter -fprofile-arcs -ftest-coverage)
    target_link_libraries(${TESTING_NAME} -lgcov --coverage)
endif ()