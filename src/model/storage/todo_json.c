#include "todo_json.h"

#include <stdio.h>
#include <string.h>

struct JSONStorage {
    struct IStorage super;
    const char *path;
};

static size_t read(const struct IStorage *self, struct Todo todos[], size_t max) {
    const struct JSONStorage *this = (const struct JSONStorage *) self;

    FILE *file = fopen(this->path, "r");
    if (file == NULL) {
        fprintf(stderr, "[CLIDemo:JSONStorage:read] Error: Failed to open file '%s' for reading: ", this->path);
        return 0;
    }

    char line[128];
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
    return i;
}

static bool write(const struct IStorage *self, const struct Todo todos[], const size_t count) {
    const struct JSONStorage *this = (struct JSONStorage *) self;

    FILE *file = fopen(this->path, "w");
    if (file == NULL) {
        fprintf(stderr, "[CLIDemo:JSONStorage:write] Error: Failed to open file '%s' for reading: ", this->path);
        return false;
    }

    fprintf(file, "[\n");
    for (size_t i = 0; i < count; i++) {
        fprintf(file, "\t{ \"id\": %u, \"title\": \"%s\", \"completed\": %s }%s\n", todos[i].id, todos[i].title, todos[i].completed ? "true" : "false", i + 1u < count ? "," : "");
    }
    fprintf(file, "]\n");

    fclose(file);
    return true;
}

static enum Status list(const struct IStorage *self, struct Todo todos[], size_t max) {
    if (self == NULL || todos == NULL || max == 0u) return ERR_INVALID_ARGS;
    return read(self, todos, max) != -1 ? OK : ERR_STORAGE_MISSING;
}

static enum Status add(const struct IStorage *self, const char *title) {
    if (self == NULL || title == NULL) return ERR_INVALID_ARGS;

    struct Todo todos[MAX_TODOS];
    size_t count = read(self, todos, MAX_TODOS);
    if (count >= MAX_TODOS) return ERR_FULL;

    struct Todo *todo = &todos[count];
    todo->id = count > 0 ? todos[count - 1].id + 1u : 1u;
    memset(todo->title, '\0', TODO_TITLE_MAX);
    strncpy(todo->title, title, TODO_TITLE_MAX - 1);
    todo->title[TODO_TITLE_MAX - 1] = '\0';
    todo->completed = false;

    return write(self, todos, count + 1u) ? OK : ERR_FULL;
}

static enum Status edit(const struct IStorage *self, const uint32_t id, const char *title, bool completed) {
    if (self == NULL) return ERR_INVALID_ARGS;

    struct Todo todos[MAX_TODOS];
    const size_t count = read(self, todos, MAX_TODOS);

    bool found = false;
    for (size_t i = 0u; i < count; i++) {
        if (todos[i].id == id) {
            if (title != NULL) {
                strncpy(todos[i].title, title, TODO_TITLE_MAX - 1);
                todos[i].title[TODO_TITLE_MAX - 1] = '\0';
            }
            todos[i].completed = completed;
            found = true;
            break;
        }
    }

    if (found == false) return ERR_NOT_FOUND;

    return write(self, todos, count);
}

static enum Status delete(const struct IStorage *self, uint32_t id) {
    if (self == NULL) return ERR_INVALID_ARGS;

    struct Todo todos[MAX_TODOS];
    const size_t count = read(self, todos, MAX_TODOS);

    bool found = false;
    size_t index = 0u;

    for (size_t i = 0u; i < count; i++) {
        if (todos[i].id == id) {
            found = true;
        } else {
            if (index != i) {
                todos[index] = todos[i]; /* shift left */
            }
            index++;
        }
    }

    if (found == false) return ERR_NOT_FOUND;

    return write(self, todos, index);
}

size_t todo_json_storage_size() {
    return (sizeof(struct JSONStorage) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

struct IStorage *todo_json_storage_init(void *buffer, const char *path) {
    struct JSONStorage *this = buffer;

    this->super.list = list;
    this->super.add = add;
    this->super.edit = edit;
    this->super.delete = delete;

    this->path = path;

    return &this->super;
}
