#pragma once

#include "puremvc/puremvc.h"
#include "components/cli.h"

#define CLIMediator_NAME "cliMediator"

struct IMediator *todo_cli_mediator_new(struct CLI *component, const char **error);
