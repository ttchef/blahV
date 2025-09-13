
#include "blahV/core/blahV_context.h"
#include "blahV/vulkan/blahV_commandPool.h"
#include "blahV/core/blahV_config.h"
#include "blahV/vulkan/blahV_device.h"
#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_pipeline.h"
#include "blahV/renderer/blahV_renderer.h"
#include "blahV/vulkan/blahV_surface.h"
#include "blahV/vulkan/blahV_swapchain.h"
#include "blahV/vulkan/blahV_window.h"
#include "blahV/renderer/blahV_rectangle.h"
#include <vulkan/vulkan_core.h>

BLV_Result blvVulkanInit(blvContext *context) {

    if (blvDeviceInit(context) != BLV_OK) return BLV_ERROR;
    
    BLV_LOG(BLV_LOG_DEBUG, "Created Devices\n");

    if (blvSurfaceInit(context) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Created Surface\n");

    if (blvSwapchainInit(context, NULL) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Created Swapchain\n");

    if (blvRectangleInit(context) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Created Rectangle Buffers");

    if (blvPipelineInit(context, blv_rectangle_binding_description, blv_rectangle_attribute_description) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Created Graphcis Pipeline\n");

    if (blvCommandPoolInit(context) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Created Command Pool/Buffers\n");

    if (blvRendererInit(context) != BLV_OK) return BLV_ERROR;

    BLV_LOG(BLV_LOG_DEBUG, "Renderer Init");

    return BLV_OK;
}

BLV_Result blvUpdate(blvContext *context) {
    blvWindowPollEvents();

    if (context->config.soft_resizing && blvWindowFinishedResize(context)) {
        blvSwapchainRecreate(context);
    }

    BLV_CHECK_RETURN(blvRendererDrawFrame(context));

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

