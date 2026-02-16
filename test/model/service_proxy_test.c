#include "service_proxy_test.h"
#include "../../src/model/service_proxy.h"

#include <stdio.h>

static void beforeAll() {}
static void beforeEach() {
    resetTxt();
    resetJSON();
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
    printf("\033[1;36m[SUITE] %s\033[0m\n", "ServiceProxyTest");
    printf("\033[1;36m================================================\033[0m\n\n");

    beforeAll();
    test("testParse", testParsePointers);
    test("testParse2", testParseArray);
    test("testStringify", testStringify);
    afterAll();

    printf("\n\033[1;32m[DONE] All tests in suite finished.\033[0m\n");
    return 0;
}

void resetTxt() {
    FILE *file = fopen("../../todos.txt", "w");
    if (file == NULL) return;

    (void) fprintf(file, "1|0|Buy groceries\n");
    (void) fprintf(file, "2|0|Water the plants\n");
    (void) fprintf(file, "3|1|Read a book\n");
    (void) fprintf(file, "4|0|Write a report\n");
    (void) fprintf(file, "5|1|Watch a movie\n");

    (void) fclose(file);
}

void resetJSON() {
    FILE *file = fopen("../../todos.json", "w");
    if (file == NULL) return;

    (void) fprintf(file, "[\n");
    (void) fprintf(file, "\t{\"id\": 1, \"title\": \"Buy groceries\", \"completed\": false},\n");
    (void) fprintf(file, "\t{\"id\": 2, \"title\": \"Water the plants\", \"completed\": false},\n");
    (void) fprintf(file, "\t{\"id\": 3, \"title\": \"Read a book\", \"completed\": true},\n");
    (void) fprintf(file, "\t{\"id\": 4, \"title\": \"Write a report\", \"completed\": false},\n");
    (void) fprintf(file, "\t{\"id\": 5, \"title\": \"Watch a movie\", \"completed\": true},\n");
    (void) fprintf(file, "]\n");

    (void) fclose(file);
}

void testParsePointers() {
    const char *json =
        "["
            "{\"id\":1,\"title\":\"Buy groceries\",\"completed\":false},"
            "{\"id\":2,\"title\":\"Water the plants\",\"completed\":false},"
            "{\"id\":3,\"title\":\"Read a book\",\"completed\":true}"
        "]";

    struct Todo *todos[] = { &(struct Todo){}, &(struct Todo){}, &(struct Todo){}, NULL };
    const size_t count = todo_parsePointers(todos, json);

    for (size_t i = 0; i < count; ++i) {
        struct Todo *todo = todos[i];
        printf("ID: %u | Title: %s | Completed: %s\n", todo->id, todo->title, todo->completed ? "true" : "false");
    }
}

void testParseArray() {
    const char *json =
    "["
        "{\"id\":1,\"title\":\"Buy groceries\",\"completed\":false},"
        "{\"id\":2,\"title\":\"Water the plants\",\"completed\":false},"
        "{\"id\":3,\"title\":\"Read a book\",\"completed\":true}"
    "]";

    struct Todo todos[10];

    size_t count = todo_parseArray(todos, 10, json);

    for (size_t i = 0; i < count; i++) {
        printf("%u. [%c] %s\n",
               todos[i].id,
               todos[i].completed ? 'x' : ' ',
               todos[i].title);
    }
}

void testStringify() {
    struct Todo todo1 = {1, "Buy groceries", false};
    struct Todo todo2 = {2, "Read a book", true};
    struct Todo todo3 = {3, "Water plants", false};

    struct Todo *todos[] = { &todo1, &todo2, &todo3, NULL };

    char json[1024];
    const size_t len = todo_stringifyPointers(todos, json, sizeof(json));

    printf("JSON array (%zu chars):\n%s\n", len, json);
}