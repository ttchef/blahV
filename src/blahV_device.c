
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_context.h"
#include <vulkan/vulkan_core.h>

BLV_Result blvDeviceInit(blvContext *context) {
    
    if (blvDeviceInstanceInit(context) != BLV_OK) {
        return BLV_ERROR;
    }

    if (blvDevicePhysicalDeviceInit(context) != BLV_OK) {
        return BLV_ERROR;
    }

    if (blvDeviceLogicalDeviceInit(context) != BLV_OK) {
        return BLV_ERROR;
    }

    return BLV_OK;
}

BLV_Result blvDeviceInstanceInit(blvContext *context) {

    VkApplicationInfo appInfo = {0};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "blv application";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "blv engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2; // TODO change later

    VkInstanceCreateInfo instanceCreateInfo = {0};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    if (vkCreateInstance(&instanceCreateInfo, NULL, &context->device.instance) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_INSTANCE_ERROR, "Couldnt create vulkan instance");
        return BLV_ERROR;
    }

    return BLV_OK;
}

BLV_Result blvDevicePhysicalDeviceInit(blvContext *context) {
    
    return BLV_OK;
}

BLV_Result blvDeviceLogicalDeviceInit(blvContext *context) {

    return BLV_OK;
}

