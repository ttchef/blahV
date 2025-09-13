
#ifndef BLAHV_COMMAND_BUFFER_H
#define BLAHV_COMMAND_BUFFER_H

#include "blahV/vulkan/blahV_device.h"
#include "blahV/core/blahV_log.h"
#include <vulkan/vulkan_core.h>

typedef struct {
    VkCommandPool pool;
    VkCommandBuffer* buffers;
    
    uint32_t buffer_count;
} blvCommandPool;

BLV_Result blvCommandPoolInit(blvContext* context);
BLV_Result blvCommandBufferRecord(blvContext* context, uint32_t frame_index, uint32_t image_index);
void blvCommandPoolDeinit(blvContext* context);

#endif

