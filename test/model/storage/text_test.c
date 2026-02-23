#include "text_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "todo/i_storage.h"
#include "model/storage/text.h"
#include "model/service_proxy.h"

#include "puremvc/i_proxy.h"

static void beforeAll() {}

static void beforeEach() {
#ifdef _WIN32
    const char *command = "copy /Y ..\\..\\src\\todos.txt ..\\todos.txt";
#else
    const char *command = "cp ../../src/todos.txt ../todos.txt";
#endif
    const int result = system(command);
    if (result != 0) fprintf(stderr, "Error executing command. Exit code: %d\n", result);
}

static void afterEach() {}

static void afterAll() {}

static void test(const char *name, void (*callback)()) {
    printf("\033[0;34m[RUNNING]\033[0m %s...\n", name);
    fflush(stdout);

    beforeEach();
    callback();
    afterEach();

    printf("\033[0;32m[PASSED]\033[0m %s\n", name);
    fflush(stdout);
}

int main(void) {
    printf("\n\033[1;36m================================================\033[0m\n");
    printf("\033[1;36m[SUITE] %s\033[0m\n", "TextTest");
    printf("\033[1;36m================================================\033[0m\n\n");

    beforeAll();
    test("testList", testList);
    test("testAdd", testAdd);
    test("testEdit", testEdit);
    test("testDelete", testDelete);
    afterAll();

    printf("\n\033[1;32m[DONE] All tests in suite finished.\033[0m\n");
    return 0;
}

void testList() {
    struct IProxy *super = puremvc_proxy_init(alloca(puremvc_proxy_size()), NULL, NULL);
    struct ServiceProxy *proxy = service_proxy_bind(&(struct ServiceProxy){}, super);
    proxy->storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../todos.txt");

    struct Todo todos[MAX_TODOS] = {0};
    proxy->list(proxy, todos, MAX_TODOS);

    const struct Todo expected[] = {
        {1u, "Buy groceries", false},
        {2u, "Water the plants", false},
        {3u, "Read a book", true},
        {4u, "Write a report", false},
        {5u, "Watch a movie", true},
    };

    const size_t count = sizeof(expected) / sizeof(expected[0]);
    for (size_t i = 0; i < count; i++) {
        if (todos[i].id != expected[i].id) abort();
        if (strcmp(todos[i].title, expected[i].title) != 0) abort();
        if (todos[i].completed != expected[i].completed) abort();
    }
}

void testAdd() {
    struct IProxy *super = puremvc_proxy_init(alloca(puremvc_proxy_size()), NULL, NULL);
    struct ServiceProxy *proxy = service_proxy_bind(&(struct ServiceProxy){}, super);
    proxy->storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../todos.txt");

    proxy->add(proxy, "Finish homework");

    struct Todo todos[MAX_TODOS] = {0};
    proxy->list(proxy, todos, MAX_TODOS);

    const struct Todo expected[] = {
        {1u, "Buy groceries", false},
        {2u, "Water the plants", false},
        {3u, "Read a book", true},
        {4u, "Write a report", false},
        {5u, "Watch a movie", true},
        {6u, "Finish homework", false},
    };

    const size_t count = sizeof(expected) / sizeof(expected[0]);
    for (size_t i = 0; i < count; i++) {
        if (todos[i].id != expected[i].id) abort();
        if (strcmp(todos[i].title, expected[i].title) != 0) abort();
        if (todos[i].completed != expected[i].completed) abort();
    }
}

void testEdit() {
    struct IProxy *super = puremvc_proxy_init(alloca(puremvc_proxy_size()), NULL, NULL);
    struct ServiceProxy *proxy = service_proxy_bind(&(struct ServiceProxy){}, super);
    proxy->storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../todos.txt");

    proxy->edit(proxy, 2, "Water the garden", true);

    struct Todo todos[MAX_TODOS] = {0};
    proxy->list(proxy, todos, MAX_TODOS);

    const struct Todo expected[] = {
        {1u, "Buy groceries", false},
        {2u, "Water the garden", true},
        {3u, "Read a book", true},
        {4u, "Write a report", false},
        {5u, "Watch a movie", true},
    };

    const size_t count = sizeof(expected) / sizeof(expected[0]);
    for (size_t i = 0; i < count; i++) {
        if (todos[i].id != expected[i].id) abort();
        if (strcmp(todos[i].title, expected[i].title) != 0) abort();
        if (todos[i].completed != expected[i].completed) abort();
    }
}

void testDelete() {
    struct IProxy *super = puremvc_proxy_init(alloca(puremvc_proxy_size()), NULL, NULL);
    struct ServiceProxy *proxy = service_proxy_bind(&(struct ServiceProxy){}, super);
    proxy->storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../todos.txt");

    proxy->delete(proxy, 2);

    struct Todo todos[MAX_TODOS] = {0};
    proxy->list(proxy, todos, MAX_TODOS);

    const struct Todo expected[] = {
        {1u, "Buy groceries", false},
        {3u, "Read a book", true},
        {4u, "Write a report", false},
        {5u, "Watch a movie", true},
    };

    const size_t count = sizeof(expected) / sizeof(expected[0]);
    for (size_t i = 0; i < count; i++) {
        if (todos[i].id != expected[i].id) abort();
        if (strcmp(todos[i].title, expected[i].title) != 0) abort();
        if (todos[i].completed != expected[i].completed) abort();
    }
}
