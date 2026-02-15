#pragma once

#include "puremvc/puremvc.h"

#include "view/components/service.h"

#define STARTUP "startup"
#define SERVICE "service"

struct ApplicationFacade {
    struct IFacade *super;

    void (*startup)(struct IFacade *self, struct Service *cli);
};

struct IFacade *application_facade_getInstance(struct FacadeMap **facadeMap, const char *key);

struct ApplicationFacade *application_facade_bind(struct ApplicationFacade *facade, struct IFacade *super);
