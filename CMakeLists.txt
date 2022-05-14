cmake_minimum_required(VERSION 3.20)

set(VERSION 1.0.0)

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
    )
endif(CMAKE_CXX_COMPILER_ID STREQUAL Clang)

add_compile_definitions(
   $<$<CONFIG:RELEASE>:NDEBUG>
)

add_executable(${PROJECT_NAME} src/main.cpp ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_BINARY_DIR}/include")
# target_include_directories(${PROJECT_NAME} PRIVATE include)

set_target_properties(${PROJECT_NAME} PROPERTIES
    EXPORT_COMPILE_COMMANDS ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED True
)


add_test(NAME basic_run COMMAND ${PROJECT_NAME})
