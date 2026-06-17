#include "storage_json_test.h"

#include "todo/i_storage.h"
#include "model/data/storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void beforeAll() {}

static void beforeEach() {
#ifdef _WIN32
    const char *command = "copy /Y ..\\..\\src\\todos.json ..\\todos.json";
#else
    const char *command = "cp ../../src/todos.json ../todos.json";
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
    printf("\033[1;36m[SUITE] %s\033[0m\n", "JSONTest");
    printf("\033[1;36m================================================\033[0m\n\n");

    beforeAll();
    test("testList", testList);
    test("testAdd", testAdd);
    test("testAddMax", testAddMax);
    test("testEdit", testEdit);
    test("testDelete", testDelete);
    afterAll();

    printf("\n\033[1;32m[DONE] All tests in suite finished.\033[0m\n");
    return 0;
}

void testList(void) {
    const char *path = "../todos.json";
    struct Storage *storage = storage_new(path);
    storage->super = json_new(path);

    struct Todo todos[MAX_TODOS] = {0};
    storage->list(storage, todos, MAX_TODOS);

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

void testAdd(void) {
    const char *path = "../todos.json";
    struct Storage *storage = storage_new(path);
    storage->super = json_new(path);

    storage->add(storage, "Finish homework");

    struct Todo todos[MAX_TODOS] = {0};
    storage->list(storage, todos, MAX_TODOS);

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

void testAddMax(void) {
    const char *path = "../todos.json";
    struct Storage *storage = storage_new(path);
    storage->super = json_new(path);

    if (storage->add(storage, "Finish homework") != OK) abort();
    if (storage->add(storage, "Call Mom") != OK) abort();
    if (storage->add(storage, "Exercise") != OK) abort();
    if (storage->add(storage, "Clean the kitchen") != OK) abort();
    if (storage->add(storage, "Plan weekend trip") != OK) abort();
    if (storage->add(storage, "Read emails") != ERR_FULL) abort();

    struct Todo todos[MAX_TODOS] = {0};
    storage->list(storage, todos, MAX_TODOS);

    const struct Todo expected[] = {
        {1u, "Buy groceries", false},
        {2u, "Water the plants", false},
        {3u, "Read a book", true},
        {4u, "Write a report", false},
        {5u, "Watch a movie", true},
        {6u, "Finish homework", false},
        {7u, "Call Mom", false},
        {8u, "Exercise", false},
        {9u, "Clean the kitchen", false},
        {10u, "Plan weekend trip", false},
    };

    const size_t count = sizeof(expected) / sizeof(expected[0]);
    for (size_t i = 0; i < count; i++) {
        if (todos[i].id != expected[i].id) abort();
        if (strcmp(todos[i].title, expected[i].title) != 0) abort();
        if (todos[i].completed != expected[i].completed) abort();
    }
}

void testEdit(void) {
    const char *path = "../todos.json";
    struct Storage *storage = storage_new(path);
    storage->super = json_new(path);

    storage->edit(storage, 2, "Water the garden", true);

    struct Todo todos[MAX_TODOS] = {0};
    storage->list(storage, todos, MAX_TODOS);

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

void testDelete(void) {
    const char *path = "../todos.json";
    struct Storage *storage = storage_new(path);
    storage->super = json_new(path);

    storage->delete(storage, 2);

    struct Todo todos[MAX_TODOS] = {0};
    storage->list(storage, todos, MAX_TODOS);

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
