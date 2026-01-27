#include <stdio.h>

#include "application_facade.h"
#include "view/components/cli.h"

int main(int argc, char **argv) {
    const char *error = NULL;
    struct CLI *cli = todo_cli_new(argc, argv, &error);

    struct IFacade *facade = todo_facade_getInstance("employee_admin", &error);
    facade->initializeFacade(facade, &error);
    todo_startup(facade, cli, &error);

    return 0;
}
