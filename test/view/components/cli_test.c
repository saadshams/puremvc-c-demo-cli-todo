#include "cli_test.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../../src/view/components/cli.h"

int main() {
    test_list();
    test_add();
    test_edit();
    test_delete();
    test_help();
    test_version();
    test_no_command();
    test_max_options();
    return 0;
}

void test_list() {
    const char *error = NULL;
    struct CLI *cli = todo_cli_new(&error);
    char *argv[] = {"todo", "list"};
    struct Command command = cli->parse(2, argv, &error);

    assert(strcmp(command.subCommand.name, "list") == 0);
    assert(command.subCommand.value == NULL);

    char *argv2[] = {"todo", "list", "--a"};
    command = cli->parse(3, argv2, &error);
    assert(strcmp(command.subCommand.name, "list") == 0);
    assert(command.subCommand.value == NULL);
    assert(strcmp(command.options[0].name, "--a") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);
}

void test_add() {
    const char *error = NULL;
    struct CLI *cli = todo_cli_new(&error);
    char *argv[] = {"todo", "add", "Buy milk"};
    struct Command command = cli->parse(3, argv, &error);

    assert(strcmp(command.subCommand.name, "add") == 0);
    assert(strcmp(command.subCommand.value, "Buy milk") == 0);
}

void test_edit() {
    const char *error = NULL;
    struct CLI *cli = todo_cli_new(&error);
    char *argv[] = {"todo", "edit", "21"};
    struct Command command = cli->parse(3, argv, &error);

    assert(strcmp(command.subCommand.name, "edit") == 0);
    assert(strcmp(command.subCommand.value, "21") == 0);

    char *argv2[] = {"todo", "edit", "21", "--title", "Water the plants"};
    command = cli->parse(5, argv2, &error);

    assert(strcmp(command.subCommand.name, "edit") == 0);
    assert(strcmp(command.subCommand.value, "21") == 0);

    assert(strcmp(command.options[0].name, "--title") == 0);
    assert(strcmp(command.options[0].value, "Water the plants") == 0);

    char *argv3[] = {"todo", "edit", "21", "--title", "Water the plants", "--completed"};
    command = cli->parse(6, argv3, &error);

    assert(strcmp(command.subCommand.name, "edit") == 0);
    assert(strcmp(command.subCommand.value, "21") == 0);

    assert(strcmp(command.options[0].name, "--title") == 0);
    assert(strcmp(command.options[0].value, "Water the plants") == 0);

    assert(strcmp(command.options[1].name, "--completed") == 0);
    assert(strcmp(command.options[1].value, "true") == 0);
}

void test_delete() {
    const char *error = NULL;
    struct CLI *cli = todo_cli_new(&error);
    char *argv[] = {"todo", "delete", "42"};
    struct Command command = cli->parse(3, argv, &error);

    assert(strcmp(command.subCommand.name, "delete") == 0);
    assert(strcmp(command.subCommand.value, "42") == 0);

    char *argv2[] = {"todo", "delete", "--all", "--force"};
    command = cli->parse(4, argv2, &error);

    assert(strcmp(command.subCommand.name, "delete") == 0);
    assert(command.subCommand.value == NULL);

    assert(strcmp(command.options[0].name, "--all") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);

    assert(strcmp(command.options[1].name, "--force") == 0);
    assert(strcmp(command.options[1].value, "true") == 0);
}

void test_help() {
    const char *error = NULL;
    struct CLI *cli = todo_cli_new(&error);
    char *argv[] = {"todo", "--help"};
    struct Command command = cli->parse(2, argv, &error);

    assert(strcmp(command.options[0].name, "--help") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);

    char *argv2[] = {"todo", "-h"};
    command = cli->parse(2, argv2, &error);

    assert(strcmp(command.options[0].name, "-h") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);

    char *argv3[] = {"todo", "list", "Buy milk", "-h"};
    command = cli->parse(4, argv3, &error);

    assert(strcmp(command.subCommand.name, "list") == 0);
    assert(strcmp(command.subCommand.value, "Buy milk") == 0);
    assert(strcmp(command.options[0].name, "-h") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);

    char *argv4[] = {"todo", "list", "Buy milk", "--help"};
    command = cli->parse(4, argv4, &error);

    assert(strcmp(command.options[0].name, "--help") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);
}

void test_version() {
    const char *error = NULL;
    struct CLI *cli = todo_cli_new(&error);
    char *argv[] = {"todo", "--version"};
    struct Command command = cli->parse(2, argv, &error);

    assert(strcmp(command.options[0].name, "--version") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);
}

void test_no_command() {
    const char *error = NULL;

    struct CLI *cli = todo_cli_new(&error);
    char *argv[] = {"todo"};
    cli->parse(1, argv, &error);

    assert(error != NULL);
}

void test_max_options() {
    const char *error = NULL;
    struct CLI *cli = todo_cli_new(&error);
    char *argv[] = {"todo", "--option1", "--option2", "--option3", "--option4", "--option5", "--option6"};
    struct Command command = cli->parse(7, argv, &error); // max options 5

    assert(strcmp(command.options[0].name, "--option1") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);

    assert(strcmp(command.options[1].name, "--option2") == 0);
    assert(strcmp(command.options[1].value, "true") == 0);

    assert(strcmp(command.options[2].name, "--option3") == 0);
    assert(strcmp(command.options[2].value, "true") == 0);

    assert(strcmp(command.options[3].name, "--option4") == 0);
    assert(strcmp(command.options[3].value, "true") == 0);

    assert(strcmp(command.options[4].name, "--option5") == 0);
    assert(strcmp(command.options[4].value, "true") == 0);
}