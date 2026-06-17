#pragma once

#include <puremvc/puremvc.h>

#define ServiceMediator_NAME "ServiceMediator"

struct ServiceMediator {
    struct IMediator *super;
};

struct IMediator *service_mediator_new(void *component);

struct ServiceMediator *service_mediator_extend(struct ServiceMediator *mediator, struct IMediator *super);
