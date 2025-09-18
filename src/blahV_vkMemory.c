
#include "blahV/vulkan/blahV_vkMemory.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"
#include "blahV/vulkan/blahV_buffer.h"
#include <vulkan/vulkan_core.h>

// Staging Buffer
BLV_Result blvMemoryUploadDataToBuffer(blvContext *context, blvBuffer *buffer, void *data, uint32_t data_count) {

    blvQueue* queue = &context->graphics_queue;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;
    blvBuffer staging_buffer;

    // TODO: fix size thing
    VkDeviceSize buffer_size = sizeof(((uint32_t*)data)[0]) * data_count;
    if (blvBufferInit(context, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &staging_buffer) != BLV_OK) {
        BLV_SET_ERROR(BLV_VULKAN_BUFFER_ERROR, "Failed to create staging buffer");
        return BLV_ERROR;
    }

    // Copy data to staging buffer
    void* mapped;
    if (vkMapMemory(context->device.logical_device, staging_buffer.memory, 0, buffer_size, 0, &mapped) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_MEMORY_ERROR, "Failed to map staging buffer memory");
        return BLV_ERROR;
    }
    memcpy(mapped, data, buffer_size);
    vkUnmapMemory(context->device.logical_device, staging_buffer.memory);

    // Create Command buffer and pool for copy command
    VkCommandPoolCreateInfo pool_create_info = {0};
    pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_create_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT; // One time use
    pool_create_info.queueFamilyIndex = queue->family_index;

    if (vkCreateCommandPool(context->device.logical_device, &pool_create_info, NULL, &command_pool) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_POOL_ERROR, "Failed to create command pool for staging buffer");
        return BLV_ERROR;
    }

    VkCommandBufferAllocateInfo buffer_alloc_info = {0};
    buffer_alloc_info.commandPool = command_pool;
    buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    buffer_alloc_info.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(context->device.logical_device, &buffer_alloc_info, &command_buffer) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to allocate command buffer for staging buffer");
        return BLV_ERROR;
    }

    VkCommandBufferBeginInfo begin_info = {0};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // One time use
    
    if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to begin command buffer for staging buffer");
        return BLV_ERROR;
    }

    VkBufferCopy region = {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = buffer_size,
    };

    // Copy Command
    vkCmdCopyBuffer(command_buffer, staging_buffer.buffer, buffer->buffer, 1, &region);

    if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to end command buffer for staging buffer");
        return BLV_ERROR;
    }
    
    // Submit
    VkSubmitInfo submit_info = {0};
    submit_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;
    
    if (vkQueueSubmit(queue->queue, 1, &submit_info, 0) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to submit command buffer for staging buffer");
        return BLV_ERROR;
    }

    // TODO: Add better error printing here
    if (vkQueueWaitIdle(queue->queue) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_ERROR, "Failed to wait for queues");
        return BLV_ERROR;
    }
    
    vkDestroyCommandPool(context->device.logical_device, command_pool, NULL);
    blvBufferDeinit(context, &staging_buffer);

    return BLV_OK;
}

// Host Visiable
BLV_Result blvMemoryUploadDataToBufferHostVisible(blvContext *context, blvBuffer *buffer, void *data, uint32_t data_count) {

    // TODO: fix size thing
    VkDeviceSize buffer_size = sizeof(((uint32_t*)data)[0]) * data_count;
    void* mapped;
    if (vkMapMemory(context->device.logical_device, buffer->memory, 0, buffer_size, 0, &mapped) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_MEMORY_ERROR, "Failed to map buffer memory");
        return BLV_ERROR;
    }
    memcpy(mapped, data, buffer_size);
    vkUnmapMemory(context->device.logical_device, buffer->memory);

    return BLV_OK;
}


