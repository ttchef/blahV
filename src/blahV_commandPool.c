
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

    if (vkAllocateCommandBuffers(context->device.logical_device, &alloc_info, context->command_pool.buffers) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to allocate command buffers!");
        return BLV_ERROR;
    }

    return BLV_OK;
    
}

BLV_Result blvCommandBufferRecord(blvContext *context) {

    VkCommandBufferBeginInfo begin_info = {0};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = NULL;

    // TODO: change command buffers to use currentImageIndex instead later for frames in flight
    if (vkBeginCommandBuffer(context->command_pool.buffers[0], &begin_info) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to begin recording of command buffers");
        return BLV_ERROR;
    }

    VkClearValue clear_color = {
        .color = {{0.0f, 0.0f, 0.0f, 1.0f}} 
    };

    VkRenderingAttachmentInfoKHR color_attachment_info = {0};
    color_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
    color_attachment_info.imageView = context->swapchain.image_views[0]; // TODO: also frames in flight
    color_attachment_info.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
    color_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment_info.clearValue = clear_color;

    VkRenderingInfoKHR render_info = {0};
    render_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
    render_info.renderArea.offset = (VkOffset2D){0, 0};
    render_info.renderArea.extent = context->swapchain.extent;
    render_info.layerCount = 1;
    render_info.colorAttachmentCount = 1;
    render_info.pColorAttachments = &color_attachment_info;

    vkCmdBeginRenderingKHR(context->command_pool.buffers[0], &render_info);

    VkViewport viewport = {0};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = context->swapchain.extent.width;
    viewport.height = context->swapchain.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {0};
    scissor.extent = context->swapchain.extent;
    scissor.offset = (VkOffset2D){0, 0};

    VkCmdSetViewport(context->command_pool.buffers[0], 0, 1, &viewport); // TODO: also frames_in_flight
    vkCmdSetScissor(context->command_pool.buffers[0], 0, 1, &scissor); // TODO: also frames_in_flight

    // Draw Calls


    vkCmdEndRenderingKHR(context->command_pool.buffers[0]);

    if (vkEndCommandBuffer(context->command_pool.buffers[0]) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to end the recording of the command buffer");
        return BLV_ERROR;
    }


    return BLV_OK;
}

void blvCommandPoolDeinit(blvContext* context) {
    vkDestroyCommandPool(context->device.logical_device, context->command_pool.pool, NULL);
    free(context->command_pool.buffers);
    context->command_pool.buffers = NULL;
}


