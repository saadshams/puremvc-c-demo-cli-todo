#pragma once

struct IService {
    void *context;
    void (*onParse)(void *context, void *data);
};
