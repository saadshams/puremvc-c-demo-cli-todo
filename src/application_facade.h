#pragma once

#include "puremvc/puremvc.h"

#include "view/components/cli.h"

#define STARTUP "startup"
#define SERVICE "service"

struct ApplicationFacade {
    struct IFacade *base;
};

struct IFacade *todo_facade_getInstance(const char *key, const char **error);

void todo_startup(const struct IFacade *self, struct CLI *cli, const char **error);
