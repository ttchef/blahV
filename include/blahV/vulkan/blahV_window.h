
#ifndef BLAHV_WINDOW_H
#define BLAHV_WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>

#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_config.h"

typedef struct blvContext blvContext;

// Function Pointers
typedef void (*blvWindowResizeCallbackPFN)(GLFWwindow*, int, int);

typedef struct {
    uint32_t width;
    uint32_t height;
    const char* name;
    GLFWwindow* glfw_window;
    bool should_quit;
    double last_resized_time;
    bool resized;
} blvWindow;

typedef struct {
    uint32_t width;
    uint32_t height;
    const char* name;
} blvWindowCreateInfo;

BLV_Result blvWindowInit(blvContext* context, blvWindowCreateInfo* createInfo, blvConfig* config);
BLV_Result blvWindowDeinit(blvContext* context);

bool blvWindowShouldQuit(blvContext* context);
void blvWindowPollEvents(); 
void blvWindowSetResizeCallback(blvWindow window, blvWindowResizeCallbackPFN function);
bool blvWindowFinishedResize(blvContext* context);

#endif
