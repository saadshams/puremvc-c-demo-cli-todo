#include "integration_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void beforeAll() {}
static void beforeEach() {
#ifdef _WIN32
    system("copy ..\\..\\todos.txt .");
    system("copy ..\\..\\todos.json .");
#else
    system("cp ../../todos.txt .");
    system("cp ../../todos.json .");
#endif
}
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
    printf("\033[1;36m[SUITE] %s\033[0m\n", "IntegrationTest");
    printf("\033[1;36m================================================\033[0m\n\n");

    beforeAll();
    test("testList", testList);
    test("testAdd", testAdd);
    test("testEdit", testEdit);
    test("testDelete", testDelete);
    afterAll();

    printf("\n\033[1;32m[DONE] All tests in suite finished.\033[0m\n");
    return 0;
}

#define MAX_OUTPUT 512

void testList() {
    char output[MAX_OUTPUT];

    {
        FILE *pipe = popen("../todo list", "r");
        if (!pipe) { perror("popen"); exit(1); }
        size_t len = 0; output[0] = '\0';
        while (fgets(output + len, sizeof(output) - len, pipe)) len = strlen(output);
        pclose(pipe);
    }

    // printf("[TEST edit] Output:\n%s\n", output);
    if(strstr(output, "Buy groceries") == NULL) abort();
    if(strstr(output, "Water the plants") == NULL) abort();
}

void testAdd() {
    char output[MAX_OUTPUT];

    {
        FILE *pipe = popen("../todo add \"Buy Milk\"", "r");
        if (!pipe) { perror("popen"); exit(1); }
        pclose(pipe);
    }

    // Verify added
    {
        FILE *pipe = popen("../todo list", "r");
        if (!pipe) { perror("popen"); exit(1); }
        size_t len = 0; output[0] = '\0';
        while (fgets(output + len, sizeof(output) - len, pipe)) len = strlen(output);
        pclose(pipe);
    }

    // printf("[TEST edit] Output:\n%s\n", output);
    if(strstr(output, "Buy Milk") == NULL) abort();
}

void testEdit() {
    char output[MAX_OUTPUT];

    // Inline run_command
    {
        FILE *pipe = popen("../todo edit 2 -t \"Water the plants - edited\" --completed", "r");
        if (!pipe) { perror("popen"); exit(1); }
        pclose(pipe);
    }

    // Verify edited
    {
        FILE *pipe = popen("../todo list", "r");
        if (!pipe) { perror("popen"); exit(1); }
        size_t len = 0; output[0] = '\0';
        while (fgets(output + len, sizeof(output) - len, pipe)) len = strlen(output);
        pclose(pipe);
    }

    // printf("[TEST edit] Output:\n%s\n", output);
    if(strstr(output, "Water the plants - edited") == NULL) abort();
}

void testDelete() {
    char output[MAX_OUTPUT];

    // Inline run_command
    {
        FILE *pipe = popen("../todo delete 1", "r");
        if (!pipe) { perror("popen"); exit(1); }
        pclose(pipe);
    }

    // Verify deletion
    {
        FILE *pipe = popen("../todo list", "r");
        if (!pipe) { perror("popen"); exit(1); }
        size_t len = 0; output[0] = '\0';
        while (fgets(output + len, sizeof(output) - len, pipe)) len = strlen(output);
        pclose(pipe);
    }

    // printf("[TEST delete] Output:\n%s\n", output);
    if(strstr(output, "Buy groceries") != NULL) abort();
}
