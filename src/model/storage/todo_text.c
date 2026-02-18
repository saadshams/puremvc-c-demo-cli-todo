#include "todo_text.h"

#include <stdio.h>
#include <string.h>

struct TextStorage {
    struct IStorage super;
    const char *path;
};

static size_t read(const struct IStorage *self, struct Todo todos[]) {
    if (todos == NULL) return 0;
    const struct TextStorage *this = (struct TextStorage *) self;

    FILE *file = fopen(this->path, "r");
    if (file == NULL) {
        fprintf(stderr, "[CLIDemo:TextStorage:read] Error: Failed to open file '%s' for reading: ", this->path);
        return false;
    }

    int i = 0;
    const char *format = "%u|%d|%[^\n]\n";
    while (i < MAX_TODOS && fscanf(file, format, &todos[i].id, (int *)&todos[i].completed, todos[i].title) == 3) {
        i++;
    }

    fclose(file);
    return i;
}

static bool write(const struct IStorage *self, const struct Todo todos[], const size_t count) {
    if (todos == NULL || count <= 0) return false;
    const struct TextStorage *this = (const struct TextStorage *) self;

    FILE *file = fopen(this->path, "w");
    if (file == NULL) {
        fprintf(stderr, "[CLIDemo:TextStorage:write] Error: Failed to open file '%s' for reading: ", this->path);
        return false;
    }

    for (size_t i = 0; i < count; i++) {
        fprintf(file, "%u|%u|%s\n", todos[i].id, todos[i].completed, todos[i].title);
    }

    fclose(file);
    return true;
}

static size_t count(const struct IStorage *self, const struct Todo *todos) {
    (void) self;
    size_t count = 0u;
    while (count < MAX_TODOS && todos[count].id != 0u) count++;
    return count;
}

static size_t list(const struct IStorage *self, struct Todo todos[]) {
    return self->read(self, todos);
}

static enum TodoStatus add(const struct IStorage *self, struct Todo todos[], const char *title) {
    if (todos == NULL) return TODO_ERR_INVALID_ARGS;
    if (self->read(self, todos) < 0) return TODO_ERR_INVALID_ARGS;

    size_t count = self->count(self, todos);
    if (count >= MAX_TODOS - 1) return TODO_ERR_FULL;

    struct Todo *todo = &todos[count];
    todo->id = count > 0 ? todos[count - 1].id + 1 : 1;
    strncpy(todo->title, title, TODO_TITLE_MAX - 1);
    todo->title[TODO_TITLE_MAX - 1] = '\0';
    todo->completed = false;

    count++;

    return self->write(self, todos, count) == true ? TODO_OK : TODO_ERR_FULL;
}

static enum TodoStatus edit(const struct IStorage *self, struct Todo todos[], uint32_t id, const char *title, const bool completed) {
    if (self->read(self, todos) < 0) return TODO_ERR_INVALID_ARGS;

    const size_t count = self->count(self, todos);

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

    if (!found) return false;

    return self->write(self, todos, count);
}

static enum TodoStatus delete(const struct IStorage *self, struct Todo todos[], uint32_t id) {
    if (self->read(self, todos) < 0) return TODO_ERR_INVALID_ARGS;

    const size_t count = self->count(self, todos);
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

    if (!found) return false;

    return self->write(self, todos, index);
}

size_t todo_text_storage_size() {
    return (sizeof(struct TextStorage) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

struct IStorage *todo_text_storage_init(void *buffer, const char *path) {
    struct TextStorage *this = buffer;

    this->super.read = read;
    this->super.write = write;

    this->super.count = count;
    this->super.list = list;
    this->super.add = add;
    this->super.edit = edit;
    this->super.delete = delete;

    this->path = path; // todo copy path

    return &this->super;
}
