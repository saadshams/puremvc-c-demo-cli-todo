#include <stdio.h>

#include "view/components/cli.h"

int main(int argc, char **argv) {
    const char *error = NULL;

    struct CLI *cli = todo_cli_new(&error);
    if (error != NULL) return printf("%s\n", error);

    cli->parse(argc, argv, &error);
    if (error != NULL) return printf("%s\n", error);

    return 0;
}
