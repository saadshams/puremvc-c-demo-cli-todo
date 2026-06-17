#include "json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static enum Status read(const struct IStorage *self, struct Todo todos[], const size_t max) {
    const struct JSONStorage *this = (const struct JSONStorage *) self;

    FILE *file = fopen(this->path, "r");
    if (file == NULL) {
        fprintf(stderr, "[Todo:JSONStorage:read] Error: Failed to open file '%s' for reading: ", this->path);
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
        fprintf(stderr, "[Todo:JSONStorage:write] Error: Failed to open file '%s' for writing: ", this->path);
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

static size_t size() {
    return (sizeof(struct JSONStorage) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

static struct IStorage *alloc() {
    struct IStorage *storage = malloc(size());

    if (storage == NULL) {
        fprintf(stderr, "\033[0;31m[Todo::JSONStorage::alloc] ERROR: Instance allocation failed.\033[0m\n");
        return NULL;
    }

    return storage;
}

static struct IStorage *init(struct IStorage *storage, const char *path) {
    if (storage == NULL) return NULL;

    struct JSONStorage *this = (struct JSONStorage *) storage;
    memset(this, 0, size());

    this->path = path;

    this->super.read = read;
    this->super.write = write;

    return &this->super;
}

struct IStorage *json_new(const char *path) {
    return init(alloc(), path);
}
