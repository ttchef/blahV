
#include "blahV/blahV_context.h"
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_surface.h"
#include "blahV/blahV_swapchain.h"
#include "blahV/blahV_window.h"
#include <vulkan/vulkan_core.h>

BLV_Result blvVulkanInit(blvContext *context) {
    blvDeviceInit(context);
    
    BLV_LOG(BLV_LOG_DEBUG, "Created Devices\n");

    blvSurfaceInit(context);

    BLV_LOG(BLV_LOG_DEBUG, "Created Surface\n");

    blvSwapchainInit(context, NULL);

    BLV_LOG(BLV_LOG_DEBUG, "Created Swapchain\n");

    return BLV_OK;
}

void blvDeinit(blvContext *context) {
    
    vkDeviceWaitIdle(context->device.logical_device);
    
    blvSwapchainDeinit(context);
    blvSurfaceDeinit(context);
    blvDeviceDeinit(context);
    blvWindowDeinit(context);
}

