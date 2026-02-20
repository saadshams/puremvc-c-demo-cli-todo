#pragma once

#include <puremvc/puremvc.h>

#define ServiceMediator_NAME "ServiceMediator"

struct ServiceMediator {
    struct IMediator *super;
};

struct IMediator *service_mediator_init(void *buffer, const char *name, void *component);

struct ServiceMediator *service_mediator_bind(struct ServiceMediator *mediator, struct IMediator *super);
