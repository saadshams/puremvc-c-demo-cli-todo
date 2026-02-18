add_executable(TodoTextTest model/storage/todo_text_test.c)
target_link_libraries(TodoTextTest PRIVATE core)
add_test(NAME Todo.TodoTextTest COMMAND TodoTextTest)

add_executable(TodoJSONTest model/storage/todo_json_test.c)
target_link_libraries(TodoJSONTest PRIVATE core)
add_test(NAME Todo.TodoJSONTest COMMAND TodoJSONTest)

add_executable(ServiceTest view/components/service_test.c)
target_link_libraries(ServiceTest PRIVATE core)
add_test(NAME Todo.ServiceTest COMMAND ServiceTest)

add_executable(IntegrationTest integration_test.c)
target_link_libraries(IntegrationTest PRIVATE core)
add_test(NAME Todo.IntegrationTest COMMAND IntegrationTest)
