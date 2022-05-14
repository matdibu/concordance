cmake_minimum_required(VERSION 3.20)

set(VERSION 1.0.0)

project(concordance VERSION ${VERSION} LANGUAGES CXX)

add_compile_options(
    -Wall -Werror -Wextra
    $<$<CONFIG:RELEASE>:-O3>
    $<$<CONFIG:DEBUG>:-ggdb3>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    add_compile_options(
        -Weverything
    )
endif(CMAKE_CXX_COMPILER_ID STREQUAL Clang)

add_compile_definitions(
   $<$<CONFIG:RELEASE>:NDEBUG>
)

add_executable(${PROJECT_NAME} src/main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    EXPORT_COMPILE_COMMANDS ON
    CXX_STANDARD_REQUIRED 17
)

