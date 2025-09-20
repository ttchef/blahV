
#include "blahV/core/blahV_config.h"
#include "blahV/core/blahV_context.h"
#include "blahV/vulkan/blahV_window.h"
#include "blahV/vulkan/blahV_device.h"
#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_swapchain.h"
#include "blahV/core/blahV_utils.h"

#include <GLFW/glfw3.h>

// Default Window Resize Callback 
void blvWindowResizeCallbackDefault(GLFWwindow* window, int width, int height) {
    blvContext* context = (blvContext*)glfwGetWindowUserPointer(window);
    context->window.width = width;
    context->window.height = height;
    blvSwapchainRecreate(context);
}

void blvWindowResizeCallbackSoftResizing(GLFWwindow* window, int width, int height) {
    blvContext* context = (blvContext*)glfwGetWindowUserPointer(window);
    context->window.last_resized_time = glfwGetTime();
    context->window.resized = true;
}


BLV_Result blvWindowInit(blvContext* context, blvWindowCreateInfo* createInfo) {

    if (!glfwInit()) {
        BLV_SET_ERROR(BLV_GLFW_ERROR, "Failed to init glfw!");
        return BLV_ERROR;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    if (createInfo->width == 0) {
        context->window.width = 800;
    }
    else {
        context->window.width = createInfo->width;
    }

    if (createInfo->height == 0) {
        context->window.height = 600;
    }
    else {
        context->window.height = createInfo->height;
    }

    if (createInfo->name == NULL) {
        context->window.name = "BLV Window";
    }
    else {
        context->window.name = createInfo->name;
    }
    
    glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    context->window.glfw_window = glfwCreateWindow(context->window.width, context->window.height, context->window.name, NULL, NULL);
    if (!context->window.glfw_window) {
        BLV_SET_ERROR(BLV_GLFW_ERROR, "Failed to create a glfw window!");
        return BLV_ERROR;
    }

    context->window.last_resized_time = 0.0;
    context->window.resized = false;

    // Set Context to User Data 
    glfwSetWindowUserPointer(context->window.glfw_window, context);
    
    // Resize Callback 
    if (context->config.soft_resizing) blvWindowSetResizeCallback(context->window, blvWindowResizeCallbackSoftResizing);
    else blvWindowSetResizeCallback(context->window, blvWindowResizeCallbackDefault);

    
    return BLV_OK;

}

bool blvWindowShouldQuit(blvContext* context) {
    return glfwWindowShouldClose(context->window.glfw_window);
}

void blvWindowPollEvents() {
    glfwPollEvents();
}

BLV_Result blvWindowDeinit(blvContext *context) {
    
    glfwDestroyWindow(context->window.glfw_window);
    glfwTerminate();

    return BLV_OK;
}

void blvWindowSetResizeCallback(blvWindow window, blvWindowResizeCallbackPFN function) {
    glfwSetWindowSizeCallback(window.glfw_window, function);
}

bool blvWindowFinishedResize(blvContext* context) {
    if (context->window.resized &&  (glfwGetTime() - context->window.last_resized_time) > 0.2) {
        context->window.resized = false;
        return true;
    }
    return false;
}

bool blvWindowGetKeyState(blvContext *context, int32_t key, int32_t state) {
    if (glfwGetKey(context->window.glfw_window, key) == state) return true;
    return false;
}

double blvWindowGetTime() {
    return glfwGetTime();
}

BLV_Result blvWindowGetCursorPosition(blvContext *context, double *pos_x, double *pos_y) {
    if (!pos_x || !pos_y) {
        BLV_SET_ERROR(BLV_INVALID_FUNCTION_INPUT, "Input Pointers are NULL");
        return BLV_ERROR;
    }
    
    glfwGetCursorPos(context->window.glfw_window, pos_x, pos_y);
    return BLV_OK;
}

void blvWindowSetInputMode(blvContext *context, int32_t mode, int32_t value) {
    glfwSetInputMode(context->window.glfw_window, mode, value);
}

void blvWindowSetCursorPos(blvContext *context, double pos_x, double pos_y) {
    glfwSetCursorPos(context->window.glfw_window, pos_x, pos_y);
}

