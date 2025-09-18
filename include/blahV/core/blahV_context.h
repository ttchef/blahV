
#ifndef BLAHV_CONTEXT_H
#define BLAHV_CONTEXT_H

#include "blahV/vulkan/blahV_commandPool.h"
#include "blahV/vulkan/blahV_queue.h"
#include "blahV/vulkan/blahV_window.h"
#include "blahV/vulkan/blahV_device.h"
#include "blahV/vulkan/blahV_surface.h"
#include "blahV/vulkan/blahV_pipeline.h"
#include "blahV/vulkan/blahV_swapchain.h"
#include "blahV/renderer/blahV_renderer.h"
#include "blahV_log.h"
#include "blahV_config.h"
#include "blahV_globals.h"
#include "blahV/vulkan/blahV_image.h"

struct blvContext {
    blvWindow window;
    blvConfig config;
    blvDevice device;
    blvQueue graphics_queue;
    blvSurface surface;
    blvSwapchain swapchain;
    blvPipeline graphcis_pipeline;
    blvCommandPool command_pool;
    blvRenderer renderer;
};

BLVAPI BLV_Result blvVulkanInit(blvContext* context);
BLV_Result blvUpdate(blvContext* context);
void blvDeinit(blvContext* context);

#endif
