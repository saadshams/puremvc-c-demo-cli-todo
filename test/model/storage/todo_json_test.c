#include "todo_json_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../src/model/storage/i_storage.h"
#include "../../../src/model/storage/todo_json.h"

static void beforeAll() {}
static void beforeEach() {}
static void afterEach() {
    reset();
}
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
    printf("\033[1;36m[SUITE] %s\033[0m\n", "TodoJSONTest");
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
    const struct IStorage *storage = todo_json_storage_init(alloca(todo_json_storage_size()), "../../todos.json");

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

void testAdd() {
    const struct IStorage *storage = todo_json_storage_init(alloca(todo_json_storage_size()), "../../todos.json");
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

void testEdit() {
    struct IStorage *storage = todo_json_storage_init(alloca(todo_json_storage_size()), "../../todos.json");
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

void testDelete() {
    struct IStorage *storage = todo_json_storage_init(alloca(todo_json_storage_size()), "../../todos.json");
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

void reset() {
    FILE *file = fopen("../../todos.json", "w");
    if (file == NULL) return;

    (void) fprintf(file, "[\n");
    (void) fprintf(file, "\t{ \"id\": 1, \"title\": \"Buy groceries\", \"completed\": false },\n");
    (void) fprintf(file, "\t{ \"id\": 2, \"title\": \"Water the plants\", \"completed\": false },\n");
    (void) fprintf(file, "\t{ \"id\": 3, \"title\": \"Read a book\", \"completed\": true },\n");
    (void) fprintf(file, "\t{ \"id\": 4, \"title\": \"Write a report\", \"completed\": false },\n");
    (void) fprintf(file, "\t{ \"id\": 5, \"title\": \"Watch a movie\", \"completed\": true }\n");
    (void) fprintf(file, "]\n");

    (void) fclose(file);
}
