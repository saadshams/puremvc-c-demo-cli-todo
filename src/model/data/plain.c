#include "plain.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static enum Status read(const struct IStorage *self, struct Todo todos[], size_t max) {
    const struct PlainStorage *this = (const struct PlainStorage *) self;

    FILE *file = fopen(this->path, "r");
    if (file == NULL) {
        fprintf(stderr, "[Todo:PlainStorage:read] Error: Failed to open file '%s' for reading: ", this->path);
        return ERR_FILE_NOT_FOUND;
    }

    size_t i = 0u;
    int completed_int;
    while (i < max) {
        memset(&todos[i], 0, sizeof(struct Todo));
        const char *format = "%u|%d|%63[^\n]";
        if (fscanf(file, format, &todos[i].id, &completed_int, todos[i].title) != 3) break;
        todos[i].completed = completed_int != 0;
        i++;
    }

    fclose(file);
    return OK;
}

static enum Status write(const struct IStorage *self, const struct Todo todos[]) {
    const struct PlainStorage *this = (const struct PlainStorage *) self;

    FILE *file = fopen(this->path, "w");
    if (file == NULL) {
        fprintf(stderr, "[Todo:PlainStorage:write] Error: Failed to open file '%s' for writing: ", this->path);
        return ERR_FILE_NOT_FOUND;
    }

    for (size_t i = 0u; todos[i].id != 0; i++) {
        fprintf(file, "%u|%u|%s\n", todos[i].id, todos[i].completed ? 1u : 0u, todos[i].title);
    }

    fclose(file);
    return OK;
}

static size_t size() {
    return (sizeof(struct PlainStorage) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

static struct IStorage *alloc() {
    struct IStorage *storage = malloc(size());

    if (storage == NULL) {
        fprintf(stderr, "\033[0;31m[Todo::PlainStorage::alloc] ERROR: Instance allocation failed.\033[0m\n");
        return NULL;
    }

    return storage;
}

static struct IStorage *init(struct IStorage *storage, const char *path) {
    if (storage == NULL) return NULL;

    struct PlainStorage *this = (struct PlainStorage *) storage;
    memset(this, 0, size());

    this->path = path;

    this->super.read = read;
    this->super.write = write;

    return &this->super;
}

struct IStorage *plain_new(const char *path) {
    return init(alloc(), path);
}
