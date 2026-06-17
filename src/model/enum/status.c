#include "status.h"

const char *status_message(enum Status status) {
    switch (status) {
        case ERR_INVALID_ID:
            return "Invalid ID. Expected a positive integer.";
        case ERR_FILE_NOT_FOUND:
            return "Database file not found. Use --file <path>.";
        case ERR_NOT_FOUND:
            return "Record not found.";
        default:
            return "Unknown error.";
    }
}