add_executable(StoragePlainTest model/data/storage_plain_test.c)
target_link_libraries(StoragePlainTest PRIVATE core)
add_test(NAME Todo.StoragePlainTest COMMAND StoragePlainTest)

add_executable(StorageJSONTest model/data/storage_json_test.c)
target_link_libraries(StorageJSONTest PRIVATE core)
add_test(NAME Todo.StorageJSONTest COMMAND StorageJSONTest)

add_executable(ArgumentTest model/valueObject/argument_test.c)
target_link_libraries(ArgumentTest PRIVATE core)
add_test(NAME Todo.ArgumentTest COMMAND ArgumentTest)

add_executable(IntegrationTest integration_test.c)
target_link_libraries(IntegrationTest PRIVATE core)
add_test(NAME Todo.IntegrationTest COMMAND IntegrationTest)
