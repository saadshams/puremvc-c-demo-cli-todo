#pragma once

#define MAX_OPTIONS 5

struct Argument {
    struct {
        const char *name;
        const char *value;
    } command;

    struct {
        const char *flag;
        const char *value;
    } options[MAX_OPTIONS];

    int count; // options count

    const char *extra; // extra argument

    const char *(*getOption)(const struct Argument *self, const char *name);
};

struct Argument *todo_argument_init(struct Argument *self);
