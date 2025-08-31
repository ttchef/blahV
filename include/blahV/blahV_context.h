
#ifndef BLAHV_CONTEXT_H
#define BLAHV_CONTEXT_H

#include "blahV/blahV_commandPool.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_queue.h"
#include "blahV_window.h"
#include "blahV_device.h"
#include "blahV_surface.h"
#include "blahV_swapchain.h"
#include "blahV_pipeline.h"
#include "blahV_config.h"

struct blvContext {
    blvWindow window;
    blvConfig config;
    blvDevice device;
    blvQueue graphics_queue;
    blvSurface surface;
    blvSwapchain swapchain;
    blvPipeline graphcis_pipeline;
    blvCommandPool command_pool;
};

BLV_Result blvVulkanInit(blvContext* context);
void blvDeinit(blvContext* context);

#endif
