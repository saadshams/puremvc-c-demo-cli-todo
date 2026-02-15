#include "service_test.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../src/view/components/service.h"

static void beforeAll() {}
static void beforeEach() {}
static void afterEach() {}
static void afterAll() {}

static void test(const char *name, void (*callback)()) {
    printf("\033[0;34m[RUNNING]\033[0m %s...\n", name);
    fflush(stdout);

    beforeEach();
    callback();
    afterEach();

    printf("\033[0;32m[PASSED]\033[0m %s\n", name);
    fflush(stdout);
}

int main(void) {
    printf("\n\033[1;36m================================================\033[0m\n");
    printf("\033[1;36m[SUITE] %s\033[0m\n", "ServiceTest");
    printf("\033[1;36m================================================\033[0m\n\n");

    beforeAll();
    test("test_list", test_list);
    test("test_list_all", test_list_all);
    test("test_add", test_add);
    test("test_edit", test_edit);
    test("test_edit_title", test_edit_title);
    test("test_edit_title_completed", test_edit_title_completed);
    test("test_delete", test_delete);
    test("test_delete_all", test_delete_all);

    test("test_help", test_help);
    test("test_help_short", test_help_short);
    test("test_help_list", test_help_list);
    test("test_version", test_version);
    test("test_no_command", test_no_command);
    test("test_max_options", test_max_options);

    afterAll();

    printf("\n\033[1;32m[DONE] All tests in suite finished.\033[0m\n");
    return 0;
}

void test_list() {
    char *argv[] = {"todo", "list"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 2, argv);
    service.parse(&service);

    assert(strcmp(command.subCommand.name, "list") == 0);
    assert(command.subCommand.value == NULL);
}

void test_list_all() {
    char *argv[] = {"todo", "list", "--a"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command,  3, argv);
    service.parse(&service);

    assert(strcmp(command.subCommand.name, "list") == 0);
    assert(command.subCommand.value == NULL);
    assert(strcmp(command.options[0].name, "--a") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);
}

void test_add() {
    char *argv[] = {"todo", "add", "Buy milk"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 3, argv);
    service.parse(&service);

    assert(strcmp(command.subCommand.name, "add") == 0);
    assert(strcmp(command.subCommand.value, "Buy milk") == 0);
}

void test_edit() {
    char *argv[] = {"todo", "edit", "21"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 3, argv);
    service.parse(&service);

    assert(strcmp(command.subCommand.name, "edit") == 0);
    assert(strcmp(command.subCommand.value, "21") == 0);
}

void test_edit_title() {
    char *argv[] = {"todo", "edit", "21", "--title", "Water the plants"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 5, argv);
    service.parse(&service);

    assert(strcmp(command.subCommand.name, "edit") == 0);
    assert(strcmp(command.subCommand.value, "21") == 0);

    assert(strcmp(command.options[0].name, "--title") == 0);
    assert(strcmp(command.options[0].value, "Water the plants") == 0);
}

void test_edit_title_completed() {
    char *argv[] = {"todo", "edit", "21", "--title", "Water the plants", "--completed"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 6, argv);
    service.parse(&service);

    assert(strcmp(command.subCommand.name, "edit") == 0);
    assert(strcmp(command.subCommand.value, "21") == 0);

    assert(strcmp(command.options[0].name, "--title") == 0);
    assert(strcmp(command.options[0].value, "Water the plants") == 0);

    assert(strcmp(command.options[1].name, "--completed") == 0);
    assert(strcmp(command.options[1].value, "true") == 0);
}

void test_delete() {
    char *argv[] = {"todo", "delete", "42"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 3, argv);
    service.parse(&service);

    assert(strcmp(command.subCommand.name, "delete") == 0);
    assert(strcmp(command.subCommand.value, "42") == 0);
}

void test_delete_all() {
    char *argv[] = {"todo", "delete", "--all", "--force"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 4, argv);
    service.parse(&service);

    assert(strcmp(command.subCommand.name, "delete") == 0);
    assert(command.subCommand.value == NULL);

    assert(strcmp(command.options[0].name, "--all") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);

    assert(strcmp(command.options[1].name, "--force") == 0);
    assert(strcmp(command.options[1].value, "true") == 0);
}

void test_help() {
    char *argv[] = {"todo", "--help"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 2, argv);
    service.parse(&service);

    assert(strcmp(command.options[0].name, "--help") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);
}

void test_help_short() {
    char *argv[] = {"todo", "-h"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 2, argv);
    service.parse(&service);

    assert(strcmp(command.options[0].name, "-h") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);
}

void test_help_list() {
    char *argv[] = {"todo", "list", "-h"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 3, argv);
    service.parse(&service);

    assert(strcmp(command.subCommand.name, "list") == 0);
    assert(command.subCommand.value == NULL);
    assert(strcmp(command.options[0].name, "-h") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);
}

void test_version() {
    char *argv[] = {"todo", "--version"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 2, argv);
    service.parse(&service);

    assert(strcmp(command.options[0].name, "--version") == 0);
    assert(strcmp(command.options[0].value, "true") == 0);
}

void test_no_command() {
    char *argv[] = {"todo"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 1, argv);
    service.parse(&service);

    // assert(error != NULL);
}

void test_max_options() {
    char *argv[] = {"todo", "--option1", "--option2", "--option3", "--option4", "--option5", "--option6"};

    struct Service service = {0};
    struct Command command = {0};

    service_init(&service, &command, 7, argv); // max options 5
    service.parse(&service);

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