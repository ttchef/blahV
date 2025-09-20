
#include "blahV/vulkan/blahV_commandPool.h"
#include "blahV/vulkan/blahV_device.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"
#include "blahV/renderer/blahV_rectangle.h"
#include "blahV/math/blahV_math.h"
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

BLV_Result blvCommandBufferRecord(blvContext *context, uint32_t frame_index, uint32_t image_index) {

    if (frame_index >= context->command_pool.buffer_count) {
        BLV_SET_ERROR(BLV_ERROR, "Image Index Invalid when recording command buffer");
        return BLV_ERROR;
    } 

    VkCommandBufferBeginInfo begin_info = {0};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = NULL;

    if (vkBeginCommandBuffer(context->command_pool.buffers[frame_index], &begin_info) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_COMMAND_BUFFER_ERROR, "Failed to begin recording of command buffers");
        return BLV_ERROR;
    }

    VkClearValue clear_color = {
        .color = {{0.0f, 0.0f, 0.0f, 1.0f}} 
    };

    VkRenderingAttachmentInfoKHR color_attachment_info = {0};
    color_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
    color_attachment_info.imageView = context->swapchain.image_views[image_index];
    color_attachment_info.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
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

    // COnvert to color attachment optimal 
    VkImageMemoryBarrier barrier_to_render = {0};
    barrier_to_render.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier_to_render.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier_to_render.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier_to_render.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier_to_render.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier_to_render.image = context->swapchain.images[image_index];
    barrier_to_render.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier_to_render.subresourceRange.layerCount = 1;
    barrier_to_render.subresourceRange.levelCount = 1;
    barrier_to_render.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    barrier_to_render.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    vkCmdPipelineBarrier(context->command_pool.buffers[frame_index], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 
                         0, 0, NULL, 0, NULL, 1, &barrier_to_render);

    vkCmdBeginRendering(context->command_pool.buffers[frame_index], &render_info);

    // Bind Pipeline
    vkCmdBindPipeline(context->command_pool.buffers[frame_index], VK_PIPELINE_BIND_POINT_GRAPHICS, context->graphcis_pipeline.pipeline);

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

    vkCmdSetViewport(context->command_pool.buffers[frame_index], 0, 1, &viewport);
    vkCmdSetScissor(context->command_pool.buffers[frame_index], 0, 1, &scissor);

    vkCmdBindDescriptorSets(context->command_pool.buffers[frame_index], VK_PIPELINE_BIND_POINT_GRAPHICS, context->graphcis_pipeline.layout,
                            0, 1, &context->graphcis_pipeline.descriptor_sets[frame_index], 0, NULL);

    // Draw Calls
    blvRendererRenderQueue(context, frame_index);

    vkCmdEndRendering(context->command_pool.buffers[frame_index]);

    VkImageMemoryBarrier barrier_to_present = {0};
    barrier_to_present.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier_to_present.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier_to_present.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barrier_to_present.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier_to_present.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier_to_present.image = context->swapchain.images[image_index];
    barrier_to_present.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier_to_present.subresourceRange.levelCount = 1;
    barrier_to_present.subresourceRange.layerCount = 1;
    barrier_to_present.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier_to_present.dstAccessMask = VK_ACCESS_NONE;
 
    vkCmdPipelineBarrier(context->command_pool.buffers[frame_index], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
                         0, 0, NULL, 0, NULL, 1, &barrier_to_present);


    if (vkEndCommandBuffer(context->command_pool.buffers[frame_index]) != VK_SUCCESS) {
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


