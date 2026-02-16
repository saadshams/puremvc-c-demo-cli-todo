add_executable(TodoTextTest model/storage/todo_text_test.c)
target_link_libraries(TodoTextTest PRIVATE core)
add_test(NAME Todo.TodoTextTest COMMAND TodoTextTest)

#add_executable(ServiceProxyTest model/service_proxy_test.c)
#target_link_libraries(ServiceProxyTest PRIVATE core)
#add_test(NAME Todo.ServiceProxyTest COMMAND ServiceProxyTest)

#add_executable(ServiceTest view/components/service_test.c)
#target_link_libraries(ServiceTest PRIVATE core)
#add_test(NAME Todo.ServiceTest COMMAND ServiceTest)
