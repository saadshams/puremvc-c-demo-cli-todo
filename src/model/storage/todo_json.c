#include "todo_json.h"

#include <stdio.h>
#include <string.h>

struct JSONStorage {
    struct IStorage super;
    const char *path;
};

static bool read(const struct IStorage *self, struct Todo *out, const size_t max) {
    if (out == NULL || max == 0u) return false;
    struct JSONStorage *this = (struct JSONStorage *) self;

    FILE *file = fopen(this->path, "r");
    if (file == NULL) return false;

    char line[128];
    size_t index = 0;
    while (fgets(line, sizeof(line), file) != NULL && index < max) {
        char completed[6];
        struct Todo *todo = &out[index];
        if (sscanf(line, " { \"id\": %u , \"title\": \"%63[^\"]\", \"completed\": %5[^ },]", &todo->id, todo->title, completed) == 3) {
            todo->completed = strcmp(completed, "true") == 0;
            index++;
        }
    }
    out[index].id = 0u; // sentinel

    fclose(file);
    return true;
}

static bool write(struct IStorage *self, const struct Todo *todos, const size_t count) {
    if (todos == NULL || count == 0u) return false;
    const struct JSONStorage *this = (struct JSONStorage *) self;

    FILE *file = fopen(this->path, "w");
    if (file == NULL) return false;

    fprintf(file, "[\n");
    for (size_t i = 0; i < count; i++) {
        fprintf(file, "\t{ \"id\": %u, \"title\": \"%s\", \"completed\": %s }%s\n", todos[i].id, todos[i].title, todos[i].completed ? "true" : "false", i + 1u < count ? "," : "");
    }
    fprintf(file, "]\n");

    fclose(file);
    return true;
}

static size_t count(const struct IStorage *self, const struct Todo *todos, const size_t max) {
    size_t count = 0u;
    while (count < max && todos[count].id != 0u) count++;
    return count;
}

static void list(const struct IStorage *self, struct Todo *todos) {
    if (!self->read(self, todos, MAX_TODOS + 1)) {
        fprintf(stderr, "Failed to read todos\n");
        return;
    }

    size_t count = self->count(self, todos, MAX_TODOS + 1);
    for (size_t i = 0u; i < count; i++) {
        printf("%u | %s | %s\n", todos[i].id, todos[i].title, todos[i].completed ? "true" : "false");
    }
}

static bool add(struct IStorage *self, const char *title, struct Todo *out) {
    if (self->read(self, out, MAX_TODOS) == false) return false;

    const size_t count = self->count(self, out, MAX_TODOS - 1);
    if (count >= MAX_TODOS - 1) return false; /* no space */

    /* Generate new ID: max existing + 1 */
    unsigned int max_id = 0u;
    for (size_t i = 0; i < count; i++) {
        if (out[i].id > max_id) max_id = out[i].id;
    }

    out[count].id = max_id + 1u;
    strncpy(out[count].title, title, TODO_TITLE_MAX - 1);
    out[count].title[TODO_TITLE_MAX - 1] = '\0';
    out[count].completed = false;

    /* Sentinel */
    out[count + 1].id = 0u;

    return self->write(self, out, count + 1);
}

static bool edit(struct IStorage *self, unsigned int id, const char *new_title, bool completed, struct Todo *out) {
    if (!self->read(self, out, MAX_TODOS + 1)) return false;

    const size_t count = self->count(self, out, MAX_TODOS + 1);
    bool found = false;

    for (size_t i = 0u; i < count; i++) {
        if (out[i].id == id) {
            if (new_title != NULL) {
                strncpy(out[i].title, new_title, TODO_TITLE_MAX - 1);
                out[i].title[TODO_TITLE_MAX - 1] = '\0';
            }
            out[i].completed = completed;
            found = true;
            break;
        }
    }

    if (!found) return false;

    return self->write(self, out, count);
}

static bool delete(struct IStorage *self, unsigned int id, struct Todo *out) {
    if (!self->read(self, out, MAX_TODOS + 1)) return false;

    size_t count = self->count(self, out, MAX_TODOS + 1);
    bool found = false;

    size_t index = 0u;
    for (size_t i = 0u; i < count; i++) {
        if (out[i].id == id) {
            found = true;
            continue; /* skip this one, effectively deleting */
        }

        if (index != i) {
            out[index] = out[i]; /* shift left */
        }
        index++;
    }

    if (!found) return false;

    /* Sentinel */
    out[index].id = 0u;

    return self->write(self, out, index);
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
