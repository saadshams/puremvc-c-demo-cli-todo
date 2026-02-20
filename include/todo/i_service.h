#pragma once

struct IService {
    const void *context;
    void (*onParse)(const void *context, void *data);
};
