
#include "blahV/blahV_context.h"
#include "blahV/blahV_window.h"
#include "blahV/blahV_log.h"
#include <GLFW/glfw3.h>

BLV_Result blvWindowInit(blvContext* context, uint32_t width, uint32_t height, const char *name) {
    if (!glfwInit()) {
        BLV_SET_ERROR(BLV_GLFW_ERROR, "Failed to init glfw!");
        return BLV_ERROR;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    context->window.width = width;
    context->window.height = height;
    context->window.name = name;
    context->window.glfw_window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!context->window.glfw_window) {
        BLV_SET_ERROR(BLV_GLFW_ERROR, "Failed to create a glfw window!");
        return BLV_ERROR;
    }
    
    return BLV_OK;

}

bool blvWindowShouldQuit(blvContext* context) {
    return context->window.should_quit ? true : false;
}

BLV_Result blvWindowDeinit(blvContext *context) {
    
    glfwDestroyWindow(context->window.glfw_window);
    glfwTerminate();

    return BLV_OK;
}

