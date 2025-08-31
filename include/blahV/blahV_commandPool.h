
#ifndef BLAHV_COMMAND_BUFFER_H
#define BLAHV_COMMAND_BUFFER_H

#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include <vulkan/vulkan_core.h>

typedef struct {
    VkCommandPool pool;
    VkCommandBuffer* buffers;
    
    uint32_t buffer_count;
} blvCommandPool;

BLV_Result blvCommandPoolInit(blvContext* context);
void blvCommandPoolDeinit(blvContext* context);

#endif

