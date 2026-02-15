add_executable(ServiceProxyTest model/service_proxy_test.c)
target_link_libraries(ServiceProxyTest PRIVATE core)
add_test(NAME Todo.ServiceProxyTest COMMAND ServiceProxyTest)

add_executable(ServiceTest view/components/service_test.c)
target_link_libraries(ServiceTest PRIVATE core)
add_test(NAME Todo.ServiceTest COMMAND ServiceTest)
