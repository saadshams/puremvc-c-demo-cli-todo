cmake_minimum_required(VERSION 4.1)
project(todo C)

set(CMAKE_C_STANDARD 11)

add_library(core
        src/model/valueObject/Command.c
        src/view/components/cli.c)

add_executable(todo src/main.c)
target_link_libraries(todo PRIVATE core)

option(BUILD_TESTS "Build Tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
