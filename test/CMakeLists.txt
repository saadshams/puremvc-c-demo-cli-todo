add_executable(ServiceProxyTextTest model/service_proxy_text_test.c)
target_link_libraries(ServiceProxyTextTest PRIVATE core)
add_test(NAME Todo.ServiceProxyTextTest COMMAND ServiceProxyTextTest)

add_executable(ServiceProxyJSONTest model/service_proxy_json_test.c)
target_link_libraries(ServiceProxyJSONTest PRIVATE core)
add_test(NAME Todo.ServiceProxyJSONTest COMMAND ServiceProxyJSONTest)

add_executable(ServiceTest view/components/service_test.c)
target_link_libraries(ServiceTest PRIVATE core)
add_test(NAME Todo.ServiceTest COMMAND ServiceTest)

add_executable(IntegrationTest integration_test.c)
target_link_libraries(IntegrationTest PRIVATE core)
add_test(NAME Todo.IntegrationTest COMMAND IntegrationTest)
