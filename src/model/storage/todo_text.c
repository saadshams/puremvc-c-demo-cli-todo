#include "todo_text.h"

#include <stdio.h>
#include <string.h>

struct TextStorage {
    struct IStorage super;
    const char *path;
};

static bool read(struct IStorage *self, struct Todo *out, size_t max) {
    if (out == NULL || max == 0u) return false;
    struct TextStorage *this = (struct TextStorage *) self;

    FILE *file = fopen(this->path, "r");
    if (file == NULL) return false;

    char line[128];
    size_t index = 0;
    while (fgets(line, sizeof(line), file) != NULL && index < max) {
        unsigned int completed;
        struct Todo *todo = &out[index];
        if (sscanf(line, "%u|%u|%63[^\n]", &todo->id, &completed, todo->title) == 3) {
            todo->completed = (completed != 0u);
            index++;
        }
    }

    out[index].id = 0u; // sentinel

    fclose(file);
    return true;
}

static bool write(struct IStorage *self, const struct Todo *todos, size_t count) {
    struct TextStorage *this = (struct TextStorage *) self;

    FILE *file = fopen(this->path, "w");
    if (file == NULL) return false;

    for (size_t i = 0; i < count; i++) {
        fprintf(file, "%u|%u|%s\n", todos[i].id, todos[i].completed, todos[i].title);
    }

    fclose(file);
    return true;
}

static size_t count(const struct IStorage *self, const struct Todo *todos, size_t max) {
    size_t count = 0u;
    while (count < max && todos[count].id != 0u) {
        count++;
    }
    return count;
}

/* List all todos (prints to stdout) */
static void list(struct IStorage *self) {
    struct Todo todos[MAX_TODOS + 1] = {0};
    if (!self->read(self, todos, MAX_TODOS + 1)) {
        fprintf(stderr, "Failed to read todos\n");
        return;
    }

    size_t count = self->count(self, todos, MAX_TODOS + 1);
    for (size_t i = 0u; i < count; i++) {
        printf("%u | %s | %s\n", todos[i].id, todos[i].title, todos[i].completed ? "true" : "false");
    }
}

static bool add(struct IStorage *self, const char *title) {
    struct Todo todos[MAX_TODOS] = {0};
    if (self->read(self, todos, MAX_TODOS) == false) return false;

    const size_t count = self->count(self, todos, MAX_TODOS - 1);
    if (count >= MAX_TODOS - 1) return false; /* no space */

    /* Generate new ID: max existing + 1 */
    unsigned int max_id = 0u;
    for (size_t i = 0; i < count; i++) {
        if (todos[i].id > max_id) max_id = todos[i].id;
    }

    todos[count].id = max_id + 1u;
    strncpy(todos[count].title, title, TODO_TITLE_MAX - 1);
    todos[count].title[TODO_TITLE_MAX - 1] = '\0';
    todos[count].completed = false;

    /* Sentinel */
    todos[count + 1].id = 0u;

    return self->write(self, todos, count + 1);
}

/* Edit a todo by ID */
static bool edit(struct IStorage *self, unsigned int id, const char *new_title, bool completed) {
    struct Todo todos[MAX_TODOS + 1] = {0};
    if (!self->read(self, todos, MAX_TODOS + 1)) return false;

    const size_t count = self->count(self, todos, MAX_TODOS + 1);
    bool found = false;

    for (size_t i = 0u; i < count; i++) {
        if (todos[i].id == id) {
            if (new_title != NULL) {
                strncpy(todos[i].title, new_title, TODO_TITLE_MAX - 1);
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

/* Delete a todo by ID */
static bool delete(struct IStorage *self, unsigned int id) {
    struct Todo todos[MAX_TODOS + 1] = {0};
    if (!self->read(self, todos, MAX_TODOS + 1)) return false;

    size_t count = self->count(self, todos, MAX_TODOS + 1);
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
