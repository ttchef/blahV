
#include "blahV/blahV_surface.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_context.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

BLV_Result blvSurfaceInit(blvContext *context) {
    if (glfwCreateWindowSurface(context->device.instance, context->window.glfw_window, NULL, &context->surface.surface) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_SURFACE_ERROR, "Failed to create Surface");
        return BLV_ERROR;
    }

    return BLV_OK;
}

void blvSurfaceDeinit(blvContext *context) {
    vkDestroySurfaceKHR(context->device.instance, context->surface.surface, NULL);
}


