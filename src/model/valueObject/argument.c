#include "argument.h"

#include <string.h>

static const char *getOption(const struct Argument *self, const char *name) {
    if (name == NULL) return NULL;

    for (int i = 0; i < self->count; i++) { // search
        if (self->options[i].flag != NULL && strcmp(self->options[i].flag, name) == 0) { // match
            return self->options[i].value;
        }
    }

    return NULL;
}

struct Argument *todo_argument_init(struct Argument *self) {
    self->getOption = getOption;
    return self;
}
