add_executable(CLITest view/components/service_test.c)
target_link_libraries(CLITest PRIVATE core)
add_test(NAME Todo.CLITest COMMAND CLITest)
