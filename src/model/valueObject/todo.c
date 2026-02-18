#include "todo.h"

#include <stdio.h>
#include <string.h>

void todo_print(const struct Todo todos[], const char *type) {
    if (strcmp(type, "text") == 0) {
        for (size_t i = 0u; i < todos[i].id != 0; i++) {
            printf("%u | %s | %s\n", todos->id, todos->title, todos->completed ? "true" : "false");
        }
    } else if (strcmp(type, "json") == 0) {
        printf("[\n");
        for (size_t i = 0u; todos[i].id != 0; i++) {
            printf("\t{ \"id\": \x1b[36m%u\x1b[0m, \"title\": \x1b[32m%s\x1b[0m, \"completed\": \x1b[33m%s\x1b[0m }%s\n",
                 todos[i].id, todos[i].title, todos[i].completed ? "true" : "false", todos[i + 1].id != 0 ? "," : "");
        }
        printf("]");
    } else {
        printf("Unknown type\n");
    }
}
