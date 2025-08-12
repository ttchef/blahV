
#include "blahV/blahV_context.h"
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_window.h"
#include <vulkan/vulkan_core.h>

BLV_Result blvVulkanInit(blvContext *context) {
    blvDeviceInit(context);
}

void blvDeinit(blvContext *context) {
    
    vkDeviceWaitIdle(context->device.logical_device);
    vkDestroyDevice(context->device.logical_device, NULL);

    if (context->device.debug_callback) {
        PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
        vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context->device.instance, "vkDestroyDebugUtilsMessengerEXT");
        if (!vkDestroyDebugUtilsMessengerEXT) {
            BLV_SET_ERROR(BLV_VULKAN_FUNCTION_LOAD_ERROR, "Failed to load vkDestroyDebugUtilsMessengerEXT");
        }

        vkDestroyDebugUtilsMessengerEXT(context->device.instance, context->device.debug_callback, NULL);
    }

    vkDestroyInstance(context->device.instance, NULL);

    blvWindowDeinit(context);
}

