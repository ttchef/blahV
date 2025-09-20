
#include "blahV/vulkan/blahV_vkMemory.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"
#include "blahV/vulkan/blahV_buffer.h"
#include "blahV/vulkan/blahV_image.h"
#include <alloca.h>
#include <vulkan/vulkan_core.h>

// Staging Buffer
BLV_Result blvMemoryUploadDataToBuffer(blvContext *context, blvBuffer *buffer, void *data, uint32_t data_size) {

    blvQueue* queue = &context->graphics_queue;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;
    blvBuffer staging_buffer;

    // TODO: fix size thing
    VkDeviceSize buffer_size = data_size;
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
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;
    
    if (vkQueueSubmit(queue->queue, 1, &submit_info, 0) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to submit command buffer for staging buffer");
        return BLV_ERROR;
    }

    // TODO: add proper syncrenasation and not queueWaitIdle
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
BLV_Result blvMemoryUploadDataToBufferHostVisible(blvContext *context, blvBuffer *buffer, void *data, uint32_t data_size) {

    // TODO: fix size thing
    VkDeviceSize buffer_size = data_size;
    void* mapped;
    if (vkMapMemory(context->device.logical_device, buffer->memory, 0, buffer_size, 0, &mapped) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_MEMORY_ERROR, "Failed to map buffer memory");
        return BLV_ERROR;
    }
    memcpy(mapped, data, buffer_size);
    vkUnmapMemory(context->device.logical_device, buffer->memory);

    return BLV_OK;
}

BLV_Result blvMemoryUploadDataToImage(blvContext *context, blvImage *image, void *data, uint32_t data_size, uint32_t width, uint32_t height,
                                      VkImageLayout final_layout, VkAccessFlags dst_access_mask) {
    blvQueue* queue = &context->graphics_queue;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;
    blvBuffer staging_buffer;

    // TODO: fix size thing
    VkDeviceSize buffer_size = data_size;
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

    // Pipeline Barrier
    VkImageMemoryBarrier image_barrier = {0};
    image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    image_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    image_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // We dont want transfer also works if we put the grapchics queue 
    image_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    image_barrier.image = image->image;
    image_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    image_barrier.subresourceRange.layerCount = 1;
    image_barrier.subresourceRange.levelCount = 1;
    image_barrier.srcAccessMask = 0; // No image use till now 
    image_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; // We want to transfer the image

    vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &image_barrier);

    VkBufferImageCopy region = {
        .imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .imageSubresource.layerCount = 1,
        .imageExtent = {width, height, 1},
    };

    // Copy Command
    vkCmdCopyBufferToImage(command_buffer, staging_buffer.buffer, image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    // Convert back pipeline barrier 
    VkImageMemoryBarrier image_barrier_final = {0};
    image_barrier_final.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    image_barrier_final.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    image_barrier_final.newLayout = final_layout;
    image_barrier_final.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    image_barrier_final.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    image_barrier_final.image = image->image;
    image_barrier_final.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    image_barrier_final.subresourceRange.layerCount = 1;
    image_barrier_final.subresourceRange.levelCount = 1;
    image_barrier_final.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    image_barrier_final.dstAccessMask = dst_access_mask;

    // TODO: with proper syncronasation VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT needs to be changed i think
    vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, NULL, 0, NULL, 1, &image_barrier_final);


    if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to end command buffer for staging buffer");
        return BLV_ERROR;
    }
    
    // Submit
    VkSubmitInfo submit_info = {0};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;
    
    if (vkQueueSubmit(queue->queue, 1, &submit_info, 0) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to submit command buffer for staging buffer");
        return BLV_ERROR;
    }

    // TODO: add proper syncrenasation and not queueWaitIdle
    // TODO: Add better error printing here
    if (vkQueueWaitIdle(queue->queue) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_ERROR, "Failed to wait for queues");
        return BLV_ERROR;
    }
    
    vkDestroyCommandPool(context->device.logical_device, command_pool, NULL);
    blvBufferDeinit(context, &staging_buffer);

    return BLV_OK;

}

