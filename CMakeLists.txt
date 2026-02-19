cmake_minimum_required(VERSION 3.22)
project(todo C)

set(CMAKE_C_STANDARD 11)

# Dependencies External Subprojects
add_subdirectory("../puremvc-c-multicore-framework2" puremvc)

add_library(core
        src/application_facade.c
        src/controller/startup_command.c
        src/controller/service_command.c
        src/model/service_proxy.c
        src/model/enum/status.c
        src/model/storage/todo_text.c
        src/model/storage/todo_json.c
        src/model/valueObject/argument.c
        src/model/valueObject/todo.c
        src/view/service_mediator.c
        src/view/components/service.c)

# Compiler Sanitizers & Warnings (cross-platform, strict)
target_compile_options(core PRIVATE
        $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer>
        $<$<C_COMPILER_ID:MSVC>:/W4 /WX>)
target_link_options(core PUBLIC
        $<$<C_COMPILER_ID:GNU,Clang>:-fsanitize=address,undefined>)

target_link_libraries(core PUBLIC puremvc)
target_include_directories(core
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ../puremvc-c-multicore-framework2/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src)

# The Executable
add_executable(todo src/main.c)
target_link_libraries(todo PRIVATE core)

option(TODO_BUILD_TESTS "Build Todo Tests" ${PROJECT_IS_TOP_LEVEL})
if (TODO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()

message(STATUS "PROJECT_IS_TOP_LEVEL = ${PROJECT_IS_TOP_LEVEL}")
message(STATUS "TODO_BUILD_TESTS = ${TODO_BUILD_TESTS}")
