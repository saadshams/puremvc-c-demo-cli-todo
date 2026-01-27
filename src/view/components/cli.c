#include <stdlib.h>
#include <string.h>

#include "cli.h"

static void parse(const struct CLI *self, const char **error) {
    if (self->argc < 2) return *error = "Usage: todo <command> [options]", (void)0;

    int i = 1;
    struct Command *command = malloc(sizeof(struct Command));

    if (i < self->argc && self->argv[i][0] != '-') {
        command->subCommand.name = self->argv[i];
        i++;
    }

    if (i < self->argc && self->argv[i][0] != '-') {
        command->subCommand.value = self->argv[i];
        i++;
    }

    while (i < self->argc && command->count < MAX_OPTIONS) {
        if (self->argv[i][0] == '-') {
            command->options[command->count].name = self->argv[i];
            if (i + 1 < self->argc && self->argv[i + 1][0] != '-' ) {
                command->options[command->count].value = self->argv[i + 1];
                command->count++;
                i += 2;
            } else {
                command->options[command->count].value = "true";
                command->count++;
                i += 1;
            }
        } else {
            command->extra = self->argv[i];
            i += 1;
        }
    }

    self->delegate(self->context, command);
}

static void setDelegate(struct CLI *self, void *context, void (*delegate)(void *context, struct Command *command)) {
    self->delegate = delegate;
    self->context = context;
}

static struct CLI *init(struct CLI *cli) {
    if (cli == NULL) return NULL;
    cli->parse = parse;
    cli->setDelegate = setDelegate;
    return cli;
}

static struct CLI *alloc(int argc, char **argv, const char **error) {
    struct CLI *cli = malloc(sizeof(struct CLI));
    if (cli == NULL) return *error = "[Todo::CLI::alloc] Error: Failed to allocate Proxy", NULL;

    memset(cli, 0, sizeof(struct CLI));

    cli->argc = argc;
    cli->argv = argv;
    cli->delegate = NULL;
    cli->context = NULL;

    return cli;
}

struct CLI *todo_cli_new(int argc, char **argv, const char **error) {
    return init(alloc(argc, argv, error));
}

void todo_cli_free(struct CLI **cli) {
    if (cli == NULL || *cli == NULL) return;
    struct CLI *this = *cli;

    free(this);
    *cli = NULL;
}
