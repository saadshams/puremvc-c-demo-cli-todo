#include "todo_text_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../src/model/storage/i_storage.h"
#include "../../../src/model/storage/todo_text.h"

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
    printf("\033[1;36m[SUITE] %s\033[0m\n", "TodoTextTest");
    printf("\033[1;36m================================================\033[0m\n\n");

    beforeAll();
    test("testRead", testRead);
    test("testWrite", testWrite);
    test("testAdd", testAdd);
    test("testEdit", testEdit);
    test("testDelete", testDelete);
    // test("testParse", testParsePointers);
    // test("testParse2", testParseArray);
    afterAll();

    printf("\n\033[1;32m[DONE] All tests in suite finished.\033[0m\n");
    return 0;
}

void testRead() {
    struct Todo todos[MAX_TODOS] = {0};

    struct IStorage *storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../../todos.txt");
    if (storage->read(storage, todos) == false) {
        fprintf(stderr, "Failed to read todos\n"); abort();
    }

    struct Todo expected[] = {
        {1u, "Buy groceries", false},
        {2u, "Water the plants", false},
        {3u, "Read a book", true},
        {4u, "Write a report", false},
        {5u, "Watch a movie", true},
    };

    size_t count = sizeof(expected) / sizeof(expected[0]);
    size_t i = 0u;
    for (; i < count; i++) {
        if (todos[i].id != expected[i].id) abort();
        if (strcmp(todos[i].title, expected[i].title) != 0) abort();
        if (todos[i].completed != expected[i].completed) abort();
    }
}

void testWrite() {
    const struct Todo data[] = {
        {1u, "Buy groceries", false},
        {2u, "Water the plants", false},
        {3u, "Read a book", true}
    };

    struct IStorage *storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../../todos.txt");
    if (storage->write(storage, data, MAX_TODOS) == false) {
        fprintf(stderr, "Failed to read todos\n"); abort();
    }

    // assertions
    struct Todo todos[MAX_TODOS] = {0};
    if (storage->read(storage, todos) == false) {
        fprintf(stderr, "Failed to read todos\n"); abort();
    }

    const struct Todo expected[] = {
        {1u, "Buy groceries", false},
        {2u, "Water the plants", false},
        {3u, "Read a book", true},
    };

    size_t count = sizeof(expected) / sizeof(expected[0]);
    for (size_t i = 0; i < count; i++) {
        if (todos[i].id != expected[i].id) abort();
        if (strcmp(todos[i].title, expected[i].title) != 0) abort();
        if (todos[i].completed != expected[i].completed) abort();
    }
}

void testAdd() {
    struct Todo todos[MAX_TODOS] = {0};

    struct IStorage *storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../../todos.txt");
    storage->add(storage, todos, "Finish homework");
    if (storage->read(storage, todos) == false) {
        fprintf(stderr, "Failed to read todos\n"); abort();
    }

    const struct Todo expected[] = {
        {1u, "Buy groceries", false},
        {2u, "Water the plants", false},
        {3u, "Read a book", true},
        {4u, "Write a report", false},
        {5u, "Watch a movie", true},
        {6u, "Finish homework", false},
    };

    size_t count = sizeof(expected) / sizeof(expected[0]);
    for (size_t i = 0; i < count; i++) {
        if (todos[i].id != expected[i].id) abort();
        if (strcmp(todos[i].title, expected[i].title) != 0) abort();
        if (todos[i].completed != expected[i].completed) abort();
    }
}

void testEdit() {
    struct Todo todos[MAX_TODOS] = {0};

    struct IStorage *storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../../todos.txt");
    storage->edit(storage, todos, 2, "Water the garden", true);
    if (storage->read(storage, todos) == false) {
        fprintf(stderr, "Failed to read todos\n"); abort();
    }

    const struct Todo expected[] = {
        {1u, "Buy groceries", false},
        {2u, "Water the garden", true},
        {3u, "Read a book", true},
        {4u, "Write a report", false},
        {5u, "Watch a movie", true},
    };

    size_t count = sizeof(expected) / sizeof(expected[0]);
    for (size_t i = 0; i < count; i++) {
        if (todos[i].id != expected[i].id) abort();
        if (strcmp(todos[i].title, expected[i].title) != 0) abort();
        if (todos[i].completed != expected[i].completed) abort();
    }
}

void testDelete() {
    struct Todo todos[MAX_TODOS] = {0};

    struct IStorage *storage = todo_text_storage_init(alloca(todo_text_storage_size()), "../../todos.txt");
    storage->delete(storage, todos, 2);
    if (storage->read(storage, todos) == false) {
        fprintf(stderr, "Failed to read todos\n"); abort();
    }

    const struct Todo expected[] = {
        {1u, "Buy groceries", false},
        {3u, "Read a book", true},
        {4u, "Write a report", false},
        {5u, "Watch a movie", true},
    };

    size_t count = sizeof(expected) / sizeof(expected[0]);
    for (size_t i = 0; i < count; i++) {
        if (todos[i].id != expected[i].id) abort();
        if (strcmp(todos[i].title, expected[i].title) != 0) abort();
        if (todos[i].completed != expected[i].completed) abort();
    }
}

// void testParsePointers() {
//     const char *json =
//         "["
//             "{\"id\":1,\"title\":\"Buy groceries\",\"completed\":false},"
//             "{\"id\":2,\"title\":\"Water the plants\",\"completed\":false},"
//             "{\"id\":3,\"title\":\"Read a book\",\"completed\":true}"
//         "]";
//
//     struct Todo *todos[] = { &(struct Todo){}, &(struct Todo){}, &(struct Todo){}, NULL };
//     const size_t count = todo_parsePointers(todos, json);
//
//     for (size_t i = 0; i < count; ++i) {
//         struct Todo *todo = todos[i];
//         printf("ID: %u | Title: %s | Completed: %s\n", todo->id, todo->title, todo->completed ? "true" : "false");
//     }
// }
//
// void testParseArray() {
//     const char *json =
//     "["
//         "{\"id\":1,\"title\":\"Buy groceries\",\"completed\":false},"
//         "{\"id\":2,\"title\":\"Water the plants\",\"completed\":false},"
//         "{\"id\":3,\"title\":\"Read a book\",\"completed\":true}"
//     "]";
//
//     struct Todo todos[10];
//
//     size_t count = todo_parseArray(todos, 10, json);
//
//     for (size_t i = 0; i < count; i++) {
//         printf("%u. [%c] %s\n",
//                todos[i].id,
//                todos[i].completed ? 'x' : ' ',
//                todos[i].title);
//     }
// }

void reset() {
    FILE *file = fopen("../../todos.txt", "w");
    if (file == NULL) return;

    (void) fprintf(file, "1|0|Buy groceries\n");
    (void) fprintf(file, "2|0|Water the plants\n");
    (void) fprintf(file, "3|1|Read a book\n");
    (void) fprintf(file, "4|0|Write a report\n");
    (void) fprintf(file, "5|1|Watch a movie\n");

    (void) fclose(file);
}
