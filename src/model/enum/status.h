#pragma once

enum Status {
    /* Success */
    OK = 0,

    /* Client/Argument Errors (100-199) */
    ERR_INVALID_ARGS = 100,
    ERR_INVALID_ID,
    ERR_BUFFER_OVERFLOW,

    /* Business Logic Errors (200-299) */
    ERR_NOT_FOUND = 200,
    ERR_FULL,

    /* Infrastructure/Storage Errors (300-399) */
    ERR_STORAGE_MISSING = 300,
    ERR_STORAGE_WRITE,
    ERR_FILE_NOT_FOUND,

    /* Catch-all */
    TODO_ERR_UNKNOWN = 999,
};

const char *status_message(enum Status status);
