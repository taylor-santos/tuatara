cmake_minimum_required(VERSION 3.10)
project(tuatara)

set(CMAKE_CXX_STANDARD 17)

find_package(BISON 3.5.0)
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

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /analyze)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter)
endif ()