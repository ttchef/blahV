
#ifndef BLAHV_WINDOW_H
#define BLAHV_WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>

#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_config.h"

#include "blahV/vulkan/blahV_glfw_keys.h"

#define BLV_KEY_STATE_PRESS GLFW_PRESS
#define BLV_KEY_STATE_RELEASE GLFW_RELEASE

// TODO: Add glfw3.h enums 
#define BLV_CURSOR GLFW_CURSOR
#define BLV_CURSOR_DISABLED GLFW_CURSOR_DISABLED
#define BLV_CURSOR_NORMAL GLFW_CURSOR_NORMAL

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

BLVAPI BLV_Result blvWindowInit(blvContext* context, blvWindowCreateInfo* createInfo, blvConfig* config);
BLV_Result blvWindowDeinit(blvContext* context);

BLVAPI bool blvWindowShouldQuit(blvContext* context);
void blvWindowPollEvents(); 
BLVAPI void blvWindowSetResizeCallback(blvWindow window, blvWindowResizeCallbackPFN function);
bool blvWindowFinishedResize(blvContext* context);
BLVAPI double blvWindowGetTime();

// Input
BLVAPI bool blvWindowGetKeyState(blvContext* context, int32_t key, int32_t state);
BLVAPI BLV_Result blvWindowGetCursorPosition(blvContext* context, double* pos_x, double* pos_y);
BLVAPI void blvWindowSetInputMode(blvContext* context, int32_t mode, int32_t value);

#endif
