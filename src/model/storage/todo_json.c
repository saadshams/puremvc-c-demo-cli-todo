#include "todo_json.h"

#include <stdio.h>
#include <string.h>

struct JSONStorage {
    struct IStorage super;
    const char *path;
};

static size_t read(const struct IStorage *self, struct Todo todos[]) {
    if (todos == NULL) return 0;
    const struct JSONStorage *this = (const struct JSONStorage *) self;

    FILE *file = fopen(this->path, "r");
    if (file == NULL) {
        fprintf(stderr, "[CLIDemo:JSONStorage:read] Error: Failed to open file '%s' for reading: ", this->path);
        return 0;
    }

    char line[128];
    size_t index = 0;
    while (fgets(line, sizeof(line), file) != NULL && index < MAX_TODOS) {
        char completed[6];
        struct Todo *todo = &todos[index];
        const char* format = " { \"id\": %u , \"title\": \"%63[^\"]\", \"completed\": %5[^ },]";
        if (sscanf(line, format, &todo->id, todo->title, completed) == 3) {
            todo->completed = strcmp(completed, "true") == 0;
            index++;
        }
    }

    fclose(file);
    return true;
}

static bool write(const struct IStorage *self, const struct Todo todos[], const size_t count) {
    if (todos == NULL || count == 0u) return false;
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

static enum TodoStatus edit(const struct IStorage *self, struct Todo todos[], const uint32_t id, const char *title, bool completed) {
    if (!self->read(self, todos)) return false;

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
    if (!self->read(self, todos)) return false;

    size_t count = self->count(self, todos);
    bool found = false;

    size_t index = 0u;
    for (size_t i = 0u; i < count; i++) {
        if (todos[i].id == id) {
            found = true;
            continue; /* skip this one, effectively deleting */
        }

        if (index != i) {
            todos[index] = todos[i]; /* shift left */
        }
        index++;
    }

    if (!found) return false;

    /* Sentinel */
    todos[index].id = 0u;

    return self->write(self, todos, index);
}

size_t todo_json_storage_size() {
    return (sizeof(struct JSONStorage) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

struct IStorage *todo_json_storage_init(void *buffer, const char *path) {
    struct JSONStorage *this = buffer;

    this->super.read = read;
    this->super.write = write;

    this->super.count = count;
    this->super.list = list;
    this->super.add = add;
    this->super.edit = edit;
    this->super.delete = delete;

    this->path = path;

    return &this->super;
}
