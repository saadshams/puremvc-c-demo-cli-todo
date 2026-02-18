#include "status.h"

const char *status_message(enum Status status) {
    switch (status) {
    case OK: return "Success";
    case ERR_INVALID_ID: return "Invalid ID provided. Please use a positive number.";
    case ERR_FILE_NOT_FOUND: return "Database file missing. A new one will be created.";
    case ERR_NOT_FOUND: return "The requested task ID does not exist.";
    default: return "An unexpected error occurred.";
    }
}