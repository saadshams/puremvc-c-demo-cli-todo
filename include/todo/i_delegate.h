#pragma once

struct IDelegate {
    const void *context;
    void (*callback)(const void *context, void *data);
};