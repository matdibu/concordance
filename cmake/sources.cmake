set(SOURCE_FILES src/main.cpp src/concordance.cpp)
set(INCLUDE_DIR include)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories(${INCLUDE_DIR})

configure_file(include/config.h.in config.h)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
