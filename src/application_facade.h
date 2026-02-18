#pragma once

#include "puremvc/puremvc.h"

#include "view/components/service.h"

#define VERSION "Todo CLI Version 1.0.0"
#define STARTUP "startup"
#define SERVICE "service"
#define SERVICE_RESULT "service_result"
#define SERVICE_FAULT "service_fault"

struct ApplicationFacade {
    struct IFacade *super;

    void (*startup)(const struct ApplicationFacade *self, struct Service *service);
};

struct IFacade *application_facade_getInstance(struct FacadeMap **facadeMap, const char *key);

struct ApplicationFacade *application_facade_bind(struct ApplicationFacade *facade, struct IFacade *super);
