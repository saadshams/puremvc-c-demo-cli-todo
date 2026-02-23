#include "status.h"

const char *status_message(enum Status status) {
    switch (status) {
    case ERR_INVALID_ID: return "Invalid ID. Please provide a positive integer.";
    case ERR_FILE_NOT_FOUND: return "Database file not found.";
    case ERR_NOT_FOUND: return "Record not found.";
    default: return "An unexpected error occurred.";
    }
}