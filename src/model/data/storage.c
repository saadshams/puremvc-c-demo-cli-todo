#include "storage.h"

#include "todo/i_storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static enum Status list(const struct Storage *self, struct Todo todos[], size_t max) {
    return self->super->read(self->super, todos, max);
}

static enum Status add(const struct Storage *self, const char *title) {
    struct Todo todos[MAX_TODOS] = {0};
    const enum Status status = self->super->read(self->super, todos, MAX_TODOS);
    if (status != OK) return status;

    size_t count = 0;
    for (size_t i = 0; i < MAX_TODOS; i++) {
        if (todos[i].id == 0) break;
        count++;
    }
    if (count >= MAX_TODOS) return ERR_FULL;

    struct Todo *todo = &todos[count];
    todo->id = todos[count - 1].id + 1;
    strncpy(todo->title, title, TODO_TITLE_MAX - 1);
    todo->title[TODO_TITLE_MAX - 1] = '\0';
    todo->completed = false;

    return self->super->write(self->super, todos);
}


static enum Status edit(const struct Storage *self, uint32_t id, const char *title, const bool completed) {
    struct Todo todos[MAX_TODOS] = {0};
    const enum Status status = self->super->read(self->super, todos, MAX_TODOS);
    if (status != OK) return status;

    bool found = false;
    for (size_t i = 0u; todos[i].id != 0; i++) {
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

    return self->super->write(self->super, todos);
}

static enum Status delete(const struct Storage *self, const uint32_t id) {
    struct Todo todos[MAX_TODOS] = {0};
    const enum Status status = self->super->read(self->super, todos, MAX_TODOS);
    if (status != OK) return status;

    bool found = false;
    size_t index = 0u;

    for (size_t i = 0u; todos[i].id != 0; i++) {
        if (todos[i].id == id) {
            found = true;
        } else {
            if (index != i) {
                todos[index] = todos[i]; /* shift left */
            }
            index++;
        }
    }
    todos[index].id = 0;
    memset(todos[index].title, 0, sizeof(todos[index].title));
    todos[index].completed = false;

    if (found == false) return ERR_NOT_FOUND;

    return self->super->write(self->super, todos);
}

static size_t size() {
    return (sizeof(struct Storage) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

static struct Storage *alloc() {
    struct Storage *storage = malloc(size());

    if (storage == NULL) {
        fprintf(stderr, "\033[0;31m[Todo::Storage::alloc] ERROR: Instance allocation failed.\033[0m\n");
        return NULL;
    }

    return storage;
}

static struct Storage *init(struct Storage *this, const char *path) {
    if (this == NULL) return NULL;

    memset(this, 0, size());

    const char *extension = strrchr(path, '.');
    const char *strategy = extension && strcmp(extension, ".json") == 0 ? "json" : "text";

    if (strcmp(strategy, "json") == 0) { // Strategy Pattern
        this->super = json_new(path);
    } else {
        this->super = plain_new(path);
    }

    this->list = list;
    this->add = add;
    this->edit = edit;
    this->delete = delete;

    return this;
}

struct Storage *storage_new(const char *path) {
    return init(alloc(), path);
}

void storage_dealloc(struct Storage **storage) {
    if (storage == NULL || *storage == NULL) return;
    struct Storage *this = *storage;

    free(this->super);

    free(*storage);
    *storage = NULL;
}
