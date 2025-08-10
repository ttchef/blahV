
#ifndef BLAHV_WINDOW_H
#define BLAHV_WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "blahV_log.h"

typedef struct blvContext blvContext;

typedef struct {
    uint32_t width;
    uint32_t height;
    const char* name;

    GLFWwindow* glfw_window;
    bool should_quit;
} blvWindow;

BLV_Result blvWindowInit(blvContext* context, uint32_t width, uint32_t height, const char* name);
bool blvWindowShouldQuit(blvContext* context);
BLV_Result blvWindowDeinit(blvContext* context);

#endif
