#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Command parse(int argc, char **argv, const char **error) {
    struct Command command = {0};

    if (argc < 2) {
        return *error = "Usage: todo <command> [options]", command;
    }

    int i = 1;

    if (i < argc && argv[i][0] != '-') {
        command.subCommand.name = argv[i];
        i++;
    }

    if (i < argc && argv[i][0] != '-') {
        command.subCommand.value = argv[i];
        i++;
    }

    while (i < argc && command.count < MAX_OPTIONS) {
        if (argv[i][0] == '-') {
            command.options[command.count].name = argv[i];
            if (i+1 < argc && argv[i+1][0] != '-' ) {
                command.options[command.count].value = argv[i+1];
                i += 2;
            } else {
                command.options[command.count].value = "true";
                i += 1;
            }
        } else {
            command.extra = argv[i];
            i += 1;
        }
        command.count++;
    }

    return command;
}

static struct CLI *init(struct CLI *cli) {
    if (cli == NULL) return NULL;
    cli->parse = parse;
    return cli;
}

static struct CLI *alloc(const char **error) {
    struct CLI *cli = malloc(sizeof(struct CLI));
    if (cli == NULL) return *error = "[Todo::CLI::alloc] Error: Failed to allocate CLI", NULL;

    memset(cli, 0, sizeof(struct CLI));

    return cli;
}

struct CLI *todo_cli_new(const char **error) {
    return init(alloc(error));
}
