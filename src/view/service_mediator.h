#pragma once

#include <puremvc/puremvc.h>

#define ServiceMediator_NAME "ServiceMediator"

struct IMediator *service_mediator_init(void *buffer, const char *name, void *component);
