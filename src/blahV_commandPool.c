
#include "blahV/blahV_commandPool.h"
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_context.h"
#include "blahV/blahV_rectangle.h"
#include "blahV/blahV_math.h"
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

    /*
    // Pipeline Barrier
    VkImageMemoryBarrier2 to_color_barrier = {0};
    to_color_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    to_color_barrier.srcStageMask = VK_PIPELINE_STAGE_2_NONE; // from present
    to_color_barrier.srcAccessMask = 0;
    to_color_barrier.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    to_color_barrier.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    to_color_barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    to_color_barrier.newLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
    to_color_barrier.image = context->swapchain.images[image_index];
    to_color_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    to_color_barrier.subresourceRange.levelCount = 1;
    to_color_barrier.subresourceRange.layerCount = 1;

    VkDependencyInfo dep_to_color_info = {0};
    dep_to_color_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dep_to_color_info.imageMemoryBarrierCount = 1;
    dep_to_color_info.pImageMemoryBarriers = &to_color_barrier;

    vkCmdPipelineBarrier2(context->command_pool.buffers[frame_index], &dep_to_color_info);
    */

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
    
    // Update Uniform Buffer
    blvMat4 model_matrix = blvMat4Translate(blvV3(0.0f, 0.0f, 0.0f));

    void* mapped;
    vkMapMemory(context->device.logical_device, context->graphcis_pipeline.uniform_buffers[frame_index].memory, 0, sizeof(blvMat4), 0, &mapped);
    memcpy(mapped, &model_matrix, sizeof(model_matrix));
    vkUnmapMemory(context->device.logical_device, context->graphcis_pipeline.uniform_buffers[frame_index].memory);
    VkBuffer vertex_buffers[] = {blv_rectangle_vertex_buffer.buffer};
    VkDeviceSize offsets[] = {0};

    // Draw Calls
    blvRendererRenderQueue(context, frame_index);

    vkCmdEndRendering(context->command_pool.buffers[frame_index]);

    /*
    VkImageMemoryBarrier2 to_present_barrier = {0};
    to_present_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    to_present_barrier.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    to_present_barrier.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    to_present_barrier.dstStageMask = VK_PIPELINE_STAGE_2_NONE;
    to_present_barrier.dstAccessMask = 0;
    to_present_barrier.oldLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
    to_present_barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    to_present_barrier.image = context->swapchain.images[image_index];
    to_present_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    to_present_barrier.subresourceRange.levelCount = 1;
    to_present_barrier.subresourceRange.layerCount = 1;

    VkDependencyInfo dep_to_present_info = {0};
    dep_to_color_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dep_to_color_info.imageMemoryBarrierCount = 1;
    dep_to_color_info.pImageMemoryBarriers = &to_present_barrier;

    vkCmdPipelineBarrier2(context->command_pool.buffers[frame_index], &dep_to_present_info);
    */

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


