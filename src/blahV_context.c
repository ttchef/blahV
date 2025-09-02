
#include "blahV/blahV_context.h"
#include "blahV/blahV_commandPool.h"
#include "blahV/blahV_config.h"
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_pipeline.h"
#include "blahV/blahV_renderer.h"
#include "blahV/blahV_surface.h"
#include "blahV/blahV_swapchain.h"
#include "blahV/blahV_window.h"
#include <vulkan/vulkan_core.h>

BLV_Result blvVulkanInit(blvContext *context) {
    
    if (blvConfigInit(context) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Created Default Config\n");

    if (blvDeviceInit(context) != BLV_OK) return BLV_ERROR;
    
    BLV_LOG(BLV_LOG_DEBUG, "Created Devices\n");

    if (blvSurfaceInit(context) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Created Surface\n");

    if (blvSwapchainInit(context, NULL) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Created Swapchain\n");

    if (blvPipelineInit(context) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Created Graphcis Pipeline\n");

    if (blvCommandPoolInit(context) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Created Command Pool/Buffers\n");

    if (blvRendererInit(context) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Renderer Init");

    return BLV_OK;
}

void blvDeinit(blvContext *context) {
    
    vkDeviceWaitIdle(context->device.logical_device);
    
    blvRendererDeinit(context);
    blvCommandPoolDeinit(context);
    blvPipelineDeinit(context);
    blvSwapchainDeinit(context);
    blvSurfaceDeinit(context);
    blvDeviceDeinit(context);
    blvWindowDeinit(context);
}

