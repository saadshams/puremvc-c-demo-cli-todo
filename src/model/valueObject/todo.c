#include "todo.h"

#include <stdio.h>
#include <string.h>

void todo_print(const struct Todo todos[], const char *path) {
    const char *extension = strrchr(path, '.');
    const char *type = extension && strcmp(extension, ".json") == 0 ? "json" : "text";

    if (strcmp(type, "text") == 0) {
        for (size_t i = 0u; todos[i].id != 0; i++) {
            printf("%u | %s | %s\n", todos[i].id, todos[i].title, todos[i].completed ? "true" : "false");
        }
    } else if (strcmp(type, "json") == 0) {
        printf("[\n");
        for (size_t i = 0u; todos[i].id != 0; i++) {
            printf("  { \"id\": \x1b[36m%u\x1b[0m, \"title\": \x1b[32m%s\x1b[0m, \"completed\": \x1b[33m%s\x1b[0m }%s\n",
                 todos[i].id, todos[i].title, todos[i].completed ? "true" : "false", todos[i + 1].id != 0 ? "," : "");
        }
        printf("]\n");
    } else {
        printf("Unknown type\n");
    }
}
