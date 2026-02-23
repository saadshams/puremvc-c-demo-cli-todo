#include "text.h"

#include <stdio.h>
#include <string.h>

struct TextStorage {
    struct IStorage super;
    const char *path;
};

static enum Status read(const struct IStorage *self, struct Todo todos[], size_t max) {
    const struct TextStorage *this = (const struct TextStorage *) self;

    FILE *file = fopen(this->path, "r");
    if (file == NULL) {
        fprintf(stderr, "[CLIDemo:TextStorage:read] Error: Failed to open file '%s' for reading: ", this->path);
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
    const struct TextStorage *this = (const struct TextStorage *) self;

    FILE *file = fopen(this->path, "w");
    if (file == NULL) {
        fprintf(stderr, "[CLIDemo:TextStorage:write] Error: Failed to open file '%s' for writing: ", this->path);
        return ERR_FILE_NOT_FOUND;
    }

    for (size_t i = 0u; todos[i].id != 0; i++) {
        fprintf(file, "%u|%u|%s\n", todos[i].id, todos[i].completed ? 1u : 0u, todos[i].title);
    }

    fclose(file);
    return OK;
}

size_t todo_text_storage_size() {
    return (sizeof(struct TextStorage) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

struct IStorage *todo_text_storage_init(void *buffer, const char *path) {
    struct TextStorage *this = buffer;

    this->super.read = read;
    this->super.write = write;

    this->path = path;

    return &this->super;
}
