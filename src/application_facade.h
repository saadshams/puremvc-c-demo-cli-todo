#pragma once

#include "puremvc/puremvc.h"

#include "view/components/cli.h"

#define STARTUP "startup"

struct ApplicationFacade {
    struct IFacade *base;
};

void todo_startup(const struct IFacade *self, struct CLI *cli, const char **error);

struct IFacade *todo_facade_getInstance(const char *key, const char **error);
