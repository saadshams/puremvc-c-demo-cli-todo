#include "json.h"

#include <stdio.h>
#include <string.h>

struct JSONStorage {
    struct IStorage super;
    const char *path;
};

static enum Status read(const struct IStorage *self, struct Todo todos[], const size_t max) {
    const struct JSONStorage *this = (const struct JSONStorage *) self;

    FILE *file = fopen(this->path, "r");
    if (file == NULL) {
        fprintf(stderr, "[CLIDemo:JSONStorage:read] Error: Failed to open file '%s' for reading: ", this->path);
        return ERR_FILE_NOT_FOUND;
    }

    char line[128] = {0};
    size_t i = 0;
    while (fgets(line, sizeof(line), file) != NULL && i < max) {
        char completed[6];
        struct Todo *todo = &todos[i];
        const char* format = " { \"id\": %u , \"title\": \"%63[^\"]\", \"completed\": %5[^ },]";
        if (sscanf(line, format, &todo->id, todo->title, completed) == 3) {
            todo->completed = strcmp(completed, "true") == 0;
            i++;
        }
    }

    fclose(file);
    return OK;
}

static enum Status write(const struct IStorage *self, const struct Todo todos[]) {
    const struct JSONStorage *this = (struct JSONStorage *) self;

    FILE *file = fopen(this->path, "w");
    if (file == NULL) {
        fprintf(stderr, "[CLIDemo:JSONStorage:write] Error: Failed to open file '%s' for reading: ", this->path);
        return ERR_FILE_NOT_FOUND;
    }

    fprintf(file, "[\n");
    for (size_t i = 0; todos[i].id != 0; i++) {
        fprintf(file, "\t{ \"id\": %u, \"title\": \"%s\", \"completed\": %s }%s\n", todos[i].id, todos[i].title, todos[i].completed ? "true" : "false", todos[i+1].id != 0 ? "," : "");
    }
    fprintf(file, "]\n");

    fclose(file);
    return OK;
}

size_t todo_json_storage_size() {
    return (sizeof(struct JSONStorage) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

struct IStorage *todo_json_storage_init(void *buffer, const char *path) {
    struct JSONStorage *this = buffer;

    this->super.read = read;
    this->super.write = write;

    this->path = path;

    return &this->super;
}
