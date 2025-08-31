
#include "blahV/blahV_commandPool.h"
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_context.h"
#include <vulkan/vulkan_core.h>
#include <stdlib.h>

BLV_Result blvCommandPoolInit(blvContext *context) {
    
    // Create Command Pool
    VkCommandPoolCreateInfo command_pool_create_info = {0};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_create_info.queueFamilyIndex = context->graphics_queue.family_index;

    if (vkCreateCommandPool(context->device.logical_device, &command_pool_create_info, NULL, &context->command_pool.pool) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_POOL_ERROR, "Failed to create Command Pool");
        return BLV_ERROR;
    }

    VkCommandBufferAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = context->command_pool.pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = context->config.frames_in_flight;

    // Allocate the memory for the buffers
    context->command_pool.buffers = malloc(sizeof(VkCommandBuffer) * context->config.frames_in_flight);
    if (!context->command_pool.buffers) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to allocate command buffers!");
        return BLV_ERROR;
    }
    context->command_pool.buffer_count = context->config.frames_in_flight;

    if (vkAllocateCommandBuffers(context->device.logical_device, NULL, context->command_pool.buffers) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to allocate command buffers!");
        return BLV_ERROR;
    }
    
}

void blvCommandPoolDeinit(blvContext* context) {
    vkDestroyCommandPool(context->device.logical_device, context->command_pool.pool, NULL);
    free(context->command_pool.buffers);
    context->command_pool.buffers = NULL;
}


