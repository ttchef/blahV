
#ifndef BLAHV_BUFFER_H
#define BLAHV_BUFFER_H

#include "blahV/vulkan/blahV_device.h"
#include "blahV/vulkan/blahV_window.h"
#include "blahV/core/blahV_log.h"

#include <vulkan/vulkan_core.h>

typedef struct {
    VkBuffer buffer;
    VkDeviceMemory memory;
} blvBuffer;

// Size in bytes btw
BLV_Result blvBufferInit(blvContext* context, VkDeviceSize size, VkBufferUsageFlags usage,
                            VkMemoryPropertyFlags properties, blvBuffer* buffer);
BLV_Result blvBufferVertexCreate(blvContext* context, blvBuffer* buffer, void* vertices, uint32_t vertices_count);
BLV_Result blvBufferIndexCreate(blvContext* context, blvBuffer* buffer, void* indices, uint32_t indices_count);

void blvBufferDeinit(blvContext* context, blvBuffer* buffer);

#endif
