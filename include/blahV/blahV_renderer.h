
#ifndef BLAHV_RENDERER_H
#define BLAHV_RENDERER_H

#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include <vulkan/vulkan.h>

typedef struct {
    VkFence* in_flight_fence;
    VkSemaphore* image_available;
    VkSemaphore* render_finished;
    uint32_t frame_index;
} blvRenderer;

BLV_Result blvRendererInit(blvContext* context);
BLV_Result blvRendererDrawFrame(blvContext* context);
void blvRendererDeinit(blvContext* context);

#endif
