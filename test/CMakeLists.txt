add_executable(TextTest model/storage/text_test.c)
target_link_libraries(TextTest PRIVATE core)
add_test(NAME Todo.TextTest COMMAND TextTest)

add_executable(JSONTest model/storage/json_test.c)
target_link_libraries(JSONTest PRIVATE core)
add_test(NAME Todo.JSONTest COMMAND JSONTest)

add_executable(ServiceTest view/components/service_test.c)
target_link_libraries(ServiceTest PRIVATE core)
add_test(NAME Todo.ServiceTest COMMAND ServiceTest)

add_executable(IntegrationTest integration_test.c)
target_link_libraries(IntegrationTest PRIVATE core)
add_test(NAME Todo.IntegrationTest COMMAND IntegrationTest)
