#include "argument_test.h"
#include "model/valueObject/argument.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    printf("\033[1;36m[SUITE] %s\033[0m\n", "ArgumentTest");
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

void test_list(void) {
    char *argv[] = {"todo", "list"};

    struct Argument *argument = argument_new(2, argv);

    if(strcmp(argument->command.name, "list") != 0) abort();
    if(argument->command.value != NULL) abort();

    free(argument);
}

void test_list_all(void) {
    char *argv[] = {"todo", "list", "--a"};

    struct Argument *argument = argument_new(3, argv);
    
    if(strcmp(argument->command.name, "list") != 0) abort();
    if(argument->command.value != NULL) abort();
    if(strcmp(argument->options[0].flag, "--a") != 0) abort();
    if(strcmp(argument->options[0].value, "true") != 0) abort();
}

void test_add(void) {
    char *argv[] = {"todo", "add", "Buy milk"};

    struct Argument *argument = argument_new(3, argv);

    if(strcmp(argument->command.name, "add") != 0) abort();
    if(strcmp(argument->command.value, "Buy milk") != 0) abort();
}

void test_edit(void) {
    char *argv[] = {"todo", "edit", "21"};

    struct Argument *argument = argument_new(3, argv);

    if(strcmp(argument->command.name, "edit") != 0) abort();
    if(strcmp(argument->command.value, "21") != 0) abort();
}

void test_edit_title(void) {
    char *argv[] = {"todo", "edit", "21", "--title", "Water the plants"};

    struct Argument *argument = argument_new(5, argv);

    if(strcmp(argument->command.name, "edit") != 0) abort();
    if(strcmp(argument->command.value, "21") != 0) abort();

    if(strcmp(argument->options[0].flag, "--title") != 0) abort();
    if(strcmp(argument->options[0].value, "Water the plants") != 0) abort();
}

void test_edit_title_completed(void) {
    char *argv[] = {"todo", "edit", "21", "--title", "Water the plants", "--completed"};

    struct Argument *argument = argument_new(6, argv);

    if(strcmp(argument->command.name, "edit") != 0) abort();
    if(strcmp(argument->command.value, "21") != 0) abort();

    if(strcmp(argument->options[0].flag, "--title") != 0) abort();
    if(strcmp(argument->options[0].value, "Water the plants") != 0) abort();

    if(strcmp(argument->options[1].flag, "--completed") != 0) abort();
    if(strcmp(argument->options[1].value, "true") != 0) abort();
}

void test_delete(void) {
    char *argv[] = {"todo", "delete", "42"};

    struct Argument *argument = argument_new(3, argv);

    if(strcmp(argument->command.name, "delete") != 0) abort();
    if(strcmp(argument->command.value, "42") != 0) abort();
}

void test_delete_all(void) {
    char *argv[] = {"todo", "delete", "--all", "--force"};

    struct Argument *argument = argument_new(4, argv);

    if(strcmp(argument->command.name, "delete") != 0) abort();
    if(argument->command.value != NULL) abort();

    if(strcmp(argument->options[0].flag, "--all") != 0) abort();
    if(strcmp(argument->options[0].value, "true") != 0) abort();

    if(strcmp(argument->options[1].flag, "--force") != 0) abort();
    if(strcmp(argument->options[1].value, "true") != 0) abort();
}

void test_help(void) {
    char *argv[] = {"todo", "--help"};

    struct Argument *argument = argument_new(2, argv);

    if(strcmp(argument->options[0].flag, "--help") != 0) abort();
    if(strcmp(argument->options[0].value, "true") != 0) abort();
}

void test_help_short(void) {
    char *argv[] = {"todo", "-h"};

    struct Argument *argument = argument_new(2, argv);

    if(strcmp(argument->options[0].flag, "-h") != 0) abort();
    if(strcmp(argument->options[0].value, "true") != 0) abort();
}

void test_help_list(void) {
    char *argv[] = {"todo", "list", "-h"};

    struct Argument *argument = argument_new(3, argv);

    if(strcmp(argument->command.name, "list") != 0) abort();
    if(argument->command.value != NULL) abort();
    if(strcmp(argument->options[0].flag, "-h") != 0) abort();
    if(strcmp(argument->options[0].value, "true") != 0) abort();
}

void test_version(void) {
    char *argv[] = {"todo", "--version"};

    struct Argument *argument = argument_new(2, argv);

    if(strcmp(argument->options[0].flag, "--version") != 0) abort();
    if(strcmp(argument->options[0].value, "true") != 0) abort();
}

void test_no_command(void) {
    char *argv[] = {"todo"};

    struct Argument *argument = argument_new(1, argv);

    if(strcmp(argument->options[0].flag, "-h") != 0) abort();
    if(strcmp(argument->options[0].value, "true") != 0) abort();
}

void test_max_options(void) {
    char *argv[] = {"todo", "--option1", "--option2", "--option3", "--option4", "--option5", "--option6"};

    struct Argument *argument = argument_new(7, argv);

    if(strcmp(argument->options[0].flag, "--option1") != 0) abort();
    if(strcmp(argument->options[0].value, "true") != 0) abort();

    if(strcmp(argument->options[1].flag, "--option2") != 0) abort();
    if(strcmp(argument->options[1].value, "true") != 0) abort();

    if(strcmp(argument->options[2].flag, "--option3") != 0) abort();
    if(strcmp(argument->options[2].value, "true") != 0) abort();

    if(strcmp(argument->options[3].flag, "--option4") != 0) abort();
    if(strcmp(argument->options[3].value, "true") != 0) abort();

    if(strcmp(argument->options[4].flag, "--option5") != 0) abort();
    if(strcmp(argument->options[4].value, "true") != 0) abort();
}