#include "todo_text.h"

#include <stdio.h>
#include <string.h>

struct TextStorage {
    struct IStorage super;
    const char *path;
};

static size_t read(const struct IStorage *self, struct Todo todos[], size_t max) {
    const struct TextStorage *this = (const struct TextStorage *) self;

    FILE *file = fopen(this->path, "r");
    if (file == NULL) {
        fprintf(stderr, "[CLIDemo:TextStorage:read] Error: Failed to open file '%s' for reading: ", this->path);
        return 0;
    }

    size_t i = 0u;
    int completed_int;
    const char *format = "%u|%d|%63[^\n]\n";
    while (i < max) {
        memset(&todos[i], 0, sizeof(struct Todo));
        if (fscanf(file, format, &todos[i].id, &completed_int, todos[i].title) != 3) break;
        todos[i].completed = completed_int != 0;
        i++;
    }

    fclose(file);
    return i;
}

static enum TodoStatus write(const struct IStorage *self, const struct Todo todos[], const size_t count) {
    const struct TextStorage *this = (const struct TextStorage *) self;

    FILE *file = fopen(this->path, "w");
    if (file == NULL) {
        fprintf(stderr, "[CLIDemo:TextStorage:write] Error: Failed to open file '%s' for writing: ", this->path);
        return TODO_ERR_INVALID_ARGS;
    }

    for (size_t i = 0u; i < count; i++) {
        fprintf(file, "%u|%u|%s\n", todos[i].id, todos[i].completed ? 1u : 0u, todos[i].title);
    }

    fclose(file);
    return TODO_OK;
}

static size_t list(const struct IStorage *self, struct Todo todos[], size_t max) {
    if (self == NULL || todos == NULL || max == 0u) return 0;
    return read(self, todos, max);
}

static enum TodoStatus add(const struct IStorage *self, const char *title) {
    if (self == NULL || title == NULL) return TODO_ERR_INVALID_ARGS;

    struct Todo todos[MAX_TODOS];
    const size_t count = read(self, todos, MAX_TODOS);
    if (count >= MAX_TODOS) return TODO_ERR_FULL;

    struct Todo *todo = &todos[count];
    todo->id = count > 0 ? todos[count - 1].id + 1u : 1u;
    strncpy(todo->title, title, TODO_TITLE_MAX - 1);
    todo->title[TODO_TITLE_MAX - 1] = '\0';
    todo->completed = false;

    return write(self, todos, count + 1u);
}

static enum TodoStatus edit(const struct IStorage *self, uint32_t id, const char *title, const bool completed) {
    if (self == NULL) return TODO_ERR_INVALID_ARGS;

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

    if (found == false) return TODO_ERR_NOT_FOUND;

    return write(self, todos, count);
}

static enum TodoStatus delete(const struct IStorage *self, uint32_t id) {
    if (self == NULL) return TODO_ERR_INVALID_ARGS;

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

    if (found == false) return TODO_ERR_NOT_FOUND;

    return write(self, todos, index);
}

size_t todo_text_storage_size() {
    return (sizeof(struct TextStorage) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

struct IStorage *todo_text_storage_init(void *buffer, const char *path) {
    struct TextStorage *this = buffer;

    this->super.list = list;
    this->super.add = add;
    this->super.edit = edit;
    this->super.delete = delete;

    this->path = path;

    return &this->super;
}
