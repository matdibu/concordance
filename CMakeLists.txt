cmake_minimum_required(VERSION 3.20)

set(VERSION 0.1.0)

project(concordance VERSION ${VERSION} LANGUAGES CXX)

configure_file(include/config.h.in config.h)

enable_testing()

add_compile_options(
    -Wall -Wextra -Werror
    $<$<CONFIG:RELEASE>:-O3>
    $<$<CONFIG:DEBUG>:-ggdb3>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    add_compile_options(
        -Weverything
        -Wno-c++98-compat
    )
endif(CMAKE_CXX_COMPILER_ID STREQUAL Clang)

add_compile_definitions(
   $<$<CONFIG:RELEASE>:NDEBUG>
)

add_executable(${PROJECT_NAME} src/main.cpp ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

set_target_properties(${PROJECT_NAME} PROPERTIES
    EXPORT_COMPILE_COMMANDS ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED True
)

set(Tests 
    basic_run 
    hello_world
    two_words_in_two_sentences
)

foreach(test ${Tests})
    add_test(NAME ${test} COMMAND ${PROJECT_NAME} ../tests/${test}/input ../tests/${test}/output)
endforeach()

