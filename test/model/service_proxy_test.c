#include "service_proxy_test.h"
#include "../../src/model/service_proxy.h"

#include <stdio.h>

static void beforeAll() {}
static void beforeEach() {
    reset();
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
    printf("\033[1;36m[SUITE] %s\033[0m\n", "ServiceProxyTest");
    printf("\033[1;36m================================================\033[0m\n\n");

    beforeAll();
    test("testLoad", testLoad);
    afterAll();

    printf("\n\033[1;32m[DONE] All tests in suite finished.\033[0m\n");
    return 0;
}

void reset() {
    FILE *file = fopen("../../todo.txt", "w");
    if (file == NULL) return;

    (void) fprintf(file, "1|0|Buy groceries\n");
    (void) fprintf(file, "2|0|Water the plants\n");
    (void) fprintf(file, "3|1|Read a book\n");
    (void) fprintf(file, "4|0|Write a report\n");
    (void) fprintf(file, "5|1|Watch a movie\n");

    (void) fclose(file);
}

void testLoad() {
    struct IProxy *super = puremvc_proxy_init(alloca(puremvc_proxy_size()), NULL, NULL);
    struct ServiceProxy *proxy = service_proxy_bind(&(struct ServiceProxy){}, super);

    struct Todo todos[MAX_TODOS + 1] = {0};

    proxy->load(proxy, "../../todo.txt", todos);

    for (size_t i = 0u; todos[i].id != 0u; i++) {
        todo_print(&todos[i]);
    }
}
