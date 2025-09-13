
#ifndef BLAHV_VKINSTANCE_H
#define BLAHV_VKINSTANCE_H

#include "blahV/blahV_log.h"
#include <vulkan/vulkan.h>

typedef struct blvContext blvContext;

typedef struct {
    VkInstance instance;
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties physical_device_properties;
    VkDevice logical_device;
    VkDebugUtilsMessengerEXT debug_callback;
} blvDevice;

BLV_Result blvDeviceInit(blvContext* context);
BLV_Result blvDeviceInstanceInit(blvContext* context);
BLV_Result blvDevicePhysicalDeviceInit(blvContext* context);
BLV_Result blvDeviceLogicalDeviceInit(blvContext* context);
void blvDeviceDeinit(blvContext* context);

bool blvDevicePhysicalDeviceIsExtensionSupported(blvContext* context, const char* name);
void blvDevicePrintDeviceExtensions(blvContext* context, const char* name);

#endif
