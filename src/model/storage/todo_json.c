#include "todo_json.h"

#include <stdio.h>

struct TodoJson {
    struct IStorage super;
};

static bool json_load(struct IStorage *self, const char *path, struct Todo *out, size_t max) {
    (void)self;

    FILE *f = fopen(path, "r");
    if (!f) return false;

    char line[256];
    size_t i = 0;

    while (fgets(line, sizeof(line), f) && i < max) {
        struct Todo t;

        if (sscanf(line,
                   " { \"id\" : %u , \"completed\" : %u , \"title\" : \"%127[^\"]\" }",
                   &t.id,
                   &t.completed,
                   t.title) == 3)
        {
            out[i++] = t;
        }
    }

    fclose(f);

    if (i < max) out[i].id = 0u;

    return true;
}

static bool json_save(struct IStorage *self, const char *path, const struct Todo *todos, size_t count) {
    (void)self;

    FILE *f = fopen(path, "w");
    if (!f) return false;

    fprintf(f, "[\n");

    for (size_t i = 0; i < count; i++) {
        fprintf(f,
                "  { \"id\": %u, \"completed\": %u, \"title\": \"%s\" }%s\n",
                todos[i].id,
                todos[i].completed,
                todos[i].title,
                (i + 1 < count) ? "," : "");
    }

    fprintf(f, "]\n");

    fclose(f);
    return true;
}

static void json_destroy(struct IStorage *self)
{
    (void)self;
}

struct IStorage *todo_storage_json_create(void *buffer)
{
    struct TodoJson *this = buffer;

    this->super.load = json_load;
    this->super.save = json_save;
    this->super.destroy = json_destroy;

    return &this->super;
}
