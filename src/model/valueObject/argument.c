#include "argument.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char *getFlag(const struct Argument *self, const char *name) {
    if (name == NULL) return NULL;

    for (int i = 0; i < self->count; i++) { // search
        if (self->options[i].flag != NULL && strcmp(self->options[i].flag, name) == 0) { // match
            return self->options[i].value;
        }
    }

    return NULL;
}

static void parse(struct Argument *self, const int argc, char **argv) {
    // Default to help when no command-line arguments are provided.
    if (argc < 2) {
        self->options[self->count].flag = "-h";
        self->options[self->count].value = "true";
    }

    int i = 1;

    // Parse optional command name and command value.
    if (i < argc && argv[i][0] != '-') {
        self->command.name = argv[i];
        i++;
    }

    if (i < argc && argv[i][0] != '-') {
        self->command.value = argv[i];
        i++;
    }

    // Parse remaining arguments as options in the form:
    //   -f value
    //   -f
    while (i < argc && self->count < MAX_OPTIONS) {
        if (argv[i][0] == '-') {
            self->options[self->count].flag = argv[i];
            if (i + 1 < argc && argv[i + 1][0] != '-' ) {
                self->options[self->count].value = argv[i + 1];
                self->count++;
                i += 2;
            } else {
                self->options[self->count].value = "true";
                self->count++;
                i += 1;
            }
        }
    }

    // Preserve any unparsed argument for application-specific handling.
    if (i < argc) {
        self->extra = argv[i];
    }
}

static size_t size(void) {
    return (sizeof(struct Argument) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

static struct Argument *alloc() {
    struct Argument *argument = malloc(size());

    if (argument == NULL) {
        fprintf(stderr, "\033[0;31m[Todo::Argument::alloc] ERROR: Instance allocation failed.\033[0m\n");
        return NULL;
    }

    return argument;
}

static struct Argument *init(struct Argument *argument, const int argc, char **argv) {
    if (argument == NULL) return NULL;

    parse(argument, argc, argv);

    argument->parse = parse;
    argument->getFlag = getFlag;

    return argument;
}

struct Argument *argument_new(const int argc, char **argv) {
    return init(alloc(), argc, argv);
}
