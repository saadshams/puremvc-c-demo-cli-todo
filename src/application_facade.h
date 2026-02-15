#pragma once

#include "puremvc/puremvc.h"

#include "view/components/service.h"

#define STARTUP "startup"
#define SERVICE "service"

struct ApplicationFacade {
    struct IFacade *super;

    void (*startup)(struct IFacade *self, struct Service *cli);
};

struct IFacade *getInstance(struct ApplicationFacade *appFacade, struct FacadeMap **facadeMap, const char *key);
