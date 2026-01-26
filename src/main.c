#include <stdio.h>

#include "view/components/cli.h"

int main(int argc, char **argv) {
    struct CLI cli = todo_cli_new();

    const char *error = NULL;
    struct Command command = cli.parse(argc, argv, &error);
    if (error != NULL) return printf("%s\n", error);

    printf("%s", command.subCommand.name);

    return 0;
}
