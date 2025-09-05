
#include "blahV/blahV_context.h"
#include "blahV/blahV_window.h"
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_swapchain.h"
#include <GLFW/glfw3.h>

// Default Window Resize Callback 
void blvWindowResizeCallbackDefault(GLFWwindow* window, int width, int height) {
    blvContext* context = (blvContext*)glfwGetWindowUserPointer(window);
    blvSwapchainRecreate(context);
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
    
    context->window.glfw_window = glfwCreateWindow(context->window.width, context->window.height, context->window.name, NULL, NULL);
    if (!context->window.glfw_window) {
        BLV_SET_ERROR(BLV_GLFW_ERROR, "Failed to create a glfw window!");
        return BLV_ERROR;
    }

    // Set Context to User Data 
    glfwSetWindowUserPointer(context->window.glfw_window, context);
    
    // Resize Callback 
    blvWindowSetResizeCallback(context->window, blvWindowResizeCallbackDefault);
    
    return BLV_OK;

}

bool blvWindowShouldQuit(blvContext* context) {
    return context->window.should_quit ? true : false;
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

