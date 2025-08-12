
#include "blahV/blahV_context.h"
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_surface.h"
#include "blahV/blahV_window.h"
#include <vulkan/vulkan_core.h>

BLV_Result blvVulkanInit(blvContext *context) {
    blvDeviceInit(context);
    blvSurfaceInit(context);
}

void blvDeinit(blvContext *context) {
    
    vkDeviceWaitIdle(context->device.logical_device);
    blvSurfaceDeinit(context);
    blvDeviceDeinit(context);
    blvWindowDeinit(context);
}

