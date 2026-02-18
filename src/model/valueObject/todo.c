#include "todo.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void todo_print(const struct Todo *todo) {
    (void) printf("%u. [%c] %s\n", todo->id, todo->completed ? 'x' : ' ', todo->title);
}

bool todo_stringifyObject(const struct Todo *todo, char *out, size_t buffer) {
    if (todo == NULL || out == NULL || buffer == 0) return false;

    const char *format = "{ \"id\": %u, \"title\": \"%s\", \"completed\": %s }";
    int len = snprintf(out, buffer, format, todo->id, todo->title, todo->completed ? "true" : "false");

    return len > 0 && (size_t) len < buffer;
}

size_t todo_stringifyArray(const struct Todo *todos, size_t count, char *buffer, size_t size) {
    if (!todos || !buffer || size == 0)
        return 0;

    size_t written = 0;

    // Write opening bracket
    int n = snprintf(buffer + written, size - written, "[");
    if (n < 0 || (size_t)n >= size - written)
        return 0;

    written += (size_t) n;

    for (size_t i = 0; i < count; ++i) {

        // Add comma between objects
        if (i > 0) {
            n = snprintf(buffer + written, size - written, ",");
            if (n < 0 || (size_t)n >= size - written)
                return written;
            written += (size_t)n;
        }

        // Append object
        const char *format = "{\"id\":%u,\"title\":\"%s\",\"completed\":%s}";
        n = snprintf(buffer + written, size - written, format, todos[i].id, todos[i].title, todos[i].completed ? "true" : "false");

        if (n < 0 || (size_t)n >= size - written)
            return written;

        written += (size_t)n;
    }

    // Closing bracket
    n = snprintf(buffer + written, size - written, "]");
    if (n < 0 || (size_t)n >= size - written)
        return written;

    written += (size_t)n;

    return written;
}

size_t todo_stringifyPointers(struct Todo **todos, char *out, size_t size) {
    if (todos == NULL || out == NULL || size == 0) return 0;

    size_t len = 0;
    len += snprintf(out + len, size - len, "[\n");

    for (size_t i = 0; todos[i] != NULL; i++) {
        char buffer[MAX_TODOS];
        if (todo_stringifyObject(todos[i], buffer, sizeof(buffer)) == false)
            break;

        if (i > 0 && len < size) // Add comma if not the first object
            len += snprintf(out + len, size - len, ",\n");

        len += snprintf(out + len, size - len, "\t%s", buffer); // append the object

        if (len >= size) break; // Stop if buffer exceeded
    }

    if (len < size) {
        len += snprintf(out + len, size - len, "\n]");
    }

    return len >= size ? size - 1 : len;
}

bool todo_parseObject(struct Todo *out, const char *object) {
    if (out == NULL) return false;

    unsigned int id;
    char title[TODO_TITLE_MAX];
    char completed[6];

    const char* format = "{ \"id\":%u, \"title\":\"%63[^\"]\", \"completed\":%9[^}] }";
    const int matched = sscanf(object, format, &id, title, completed);

    if (matched != 3) return false;

    out->id = id;
    snprintf(out->title, TODO_TITLE_MAX, "%s", title);
    out->completed = strcmp(completed, "true") == 0;

    return true;
}

size_t todo_parseArray(struct Todo *out, size_t max, const char *json) {
    if (out == NULL || json == NULL || max == 0) return 0;

    size_t count = 0;
    const char *start = NULL, *end = NULL;
    for (const char *cursor = json; *cursor != '\0' && count < max; count++, cursor = end + 1) {   // move past the previous '}'
        if ((start = strchr(cursor, '{')) == NULL) break; // find object start
        if ((end = strchr(start, '}')) == NULL) break; // find object end
        size_t len = end - start + 1;

        char buffer[256];
        if (len >= sizeof(buffer)) return count;

        memcpy(buffer, start, len);
        buffer[len] = '\0';

        if (todo_parseObject(&out[count], buffer) == false) break;
    }

    return count;
}

size_t todo_parsePointers(struct Todo **out, const char *json) {
    if (out == NULL || json == NULL) return 0;

    size_t count = 0;
    const char *start = NULL, *end = NULL;
    for (const char *cursor = json; *cursor != '\0' && out[count] != NULL; count++, cursor = end + 1) {
        if ((start = strchr(cursor, '{')) == NULL) break; // find object start
        if ((end = strchr(start, '}')) == NULL) break; // find object end
        size_t len = end - start + 1;

        char buffer[256];
        if (len >= sizeof(buffer)) return count;

        memcpy(buffer, start, len);
        buffer[len] = '\0';

        if (todo_parseObject(out[count], buffer) == false) break;
    }

    return count;
}
