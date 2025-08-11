
#ifndef BLAHV_VKINSTANCE_H
#define BLAHV_VKINSTANCE_H

#include "blahV/blahV_log.h"
#include <vulkan/vulkan.h>

typedef struct blvContext blvContext;

typedef struct {
    VkInstance instance;
} blvDevice;

BLV_Result blvDeviceInit(blvContext* context);
BLV_Result blvDeviceInstanceInit(blvContext* context);
BLV_Result blvDevicePhysicalDeviceInit(blvContext* context);
BLV_Result blvDeviceLogicalDeviceInit(blvContext* context);

#endif
