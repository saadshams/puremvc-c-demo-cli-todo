add_executable(CLITest view/components/cli_test.c)
target_link_libraries(CLITest PRIVATE core)
add_test(NAME Todo.CLITest COMMAND CLITest)
