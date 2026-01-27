#pragma once

#include "puremvc/puremvc.h"
#include "valueObject/Command.h"

#define SERVICE_PROXY_NAME "service_proxy"

void todo_service_proxy_list(struct IProxy *self, struct Command *command);

void todo_service_proxy_add(struct IProxy *self, struct Command *command);

void todo_service_proxy_edit(struct IProxy *self, struct Command *command);

void todo_service_proxy_delete(struct IProxy *self, struct Command *command);

struct IProxy *todo_service_proxy_new(const char **error);
