#include "cli.h"

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
            if (i + 1 < argc && argv[i + 1][0] != '-' ) {
                command.options[command.count].value = argv[i + 1];
                command.count++;
                i += 2;
            } else {
                command.options[command.count].value = "true";
                command.count++;
                i += 1;
            }
        } else {
            command.extra = argv[i];
            i += 1;
        }
    }

    return command;
}

struct CLI todo_cli_new() {
    return (struct CLI){
        .parse = parse
    };
}
