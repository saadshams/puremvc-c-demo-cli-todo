#pragma once

#define MAX_OPTIONS 5

struct Argument {
    struct {
        const char *name;
        const char *value;
    } subCommand;

    struct {
        const char *name;
        const char *value;
    } options[MAX_OPTIONS];

    int count; // options count

    const char *extra; // extra argument
};
