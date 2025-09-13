
#ifndef BLAHV_RENDERER_H
#define BLAHV_RENDERER_H

#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include <vulkan/vulkan.h>

typedef enum {
    BLV_DRAW_TYPE_RECTANGLE = 0,
    BLV_DRAW_TYPE_SQUARE = 1,
} BLV_DrawType;

typedef struct {
    VkFence* in_flight_fence;
    VkSemaphore* image_available;
    VkSemaphore* render_finished;
    uint32_t frame_index;
    VkFence* images_in_flight;
    void** draw_calls;
    int32_t draw_calls_index;
    int32_t draw_calls_capacity;
} blvRenderer;

BLV_Result blvRendererInit(blvContext* context);
BLV_Result blvRendererPushDrawCall(blvContext* context, void* data);
BLV_Result blvRendererDrawFrame(blvContext* context);
BLV_Result blvRendererRenderQueue(blvContext* context, uint32_t index);
void blvRendererDeinit(blvContext* context);

#endif

