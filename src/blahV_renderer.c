
#include "blahV/renderer/blahV_renderer.h"
#include "blahV/vulkan/blahV_commandPool.h"
#include "blahV/vulkan/blahV_device.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"
#include "blahV/renderer/blahV_rectangle.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

BLV_Result blvRendererInit(blvContext* context) {

    // Create Infos
    VkSemaphoreCreateInfo sempahore_info = {0};
    sempahore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info = {0};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    // Allocate
    context->renderer.image_available = malloc(sizeof(VkSemaphore) * context->config.frames_in_flight);
    if (!context->renderer.image_available) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to allocate image_available semaphores");
        return BLV_ERROR;
    }

    context->renderer.render_finished = malloc(sizeof(VkSemaphore) * context->config.frames_in_flight);
    if (!context->renderer.render_finished) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to allocate render_finished semaphores");
        return BLV_ERROR;
    }

    context->renderer.in_flight_fence = malloc(sizeof(VkFence) * context->config.frames_in_flight);
    if (!context->renderer.in_flight_fence) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to allocate in_flight_fences");
        return BLV_ERROR;
    }

    // Frames in flight
    for (int32_t i = 0; i < context->config.frames_in_flight; i++) {
        if (vkCreateSemaphore(context->device.logical_device, &sempahore_info, NULL, &context->renderer.image_available[i]) != VK_SUCCESS) {
            BLV_SET_ERROR(BLV_ERROR, "Failed to create image_available semaphore");
            return BLV_ERROR;
        }

        if (vkCreateSemaphore(context->device.logical_device, &sempahore_info, NULL, &context->renderer.render_finished[i]) != VK_SUCCESS) {
            BLV_SET_ERROR(BLV_ERROR, "Failed to create render_finished semaphore");
            return BLV_ERROR;
        }

        if (vkCreateFence(context->device.logical_device, &fence_info, NULL, &context->renderer.in_flight_fence[i]) != VK_SUCCESS) {
            BLV_SET_ERROR(BLV_ERROR, "Failed to create frames in flight fence");
            return BLV_ERROR;
        }
    }

    // Images in flight
    context->renderer.images_in_flight = malloc(sizeof(VkFence) * context->swapchain.image_count);
    if (!context->renderer.images_in_flight) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to allocate renderer images in flight");
        return BLV_ERROR;
    }

    for (int32_t i = 0; i < context->swapchain.image_count; i++) {
        context->renderer.images_in_flight[i] = VK_NULL_HANDLE;
    }

    // Draw Calls
    context->renderer.draw_calls_capacity = 10;
    context->renderer.draw_calls = malloc(sizeof(void*) * context->renderer.draw_calls_capacity); 
    if (!context->renderer.draw_calls) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to allocate renderer draw calls array");
        return BLV_ERROR;
    }
    context->renderer.draw_calls_index = 0;

    // Zero Initialise
    for (int32_t i = 0; i < context->renderer.draw_calls_capacity; i++) {
        context->renderer.draw_calls[i] = NULL;
    }

    return BLV_OK;
}

BLV_Result blvRendererPushDrawCall(blvContext *context, void *data) {

    if (context->renderer.draw_calls_index >= context->renderer.draw_calls_capacity) {
        context->renderer.draw_calls_capacity *= 2;
        context->renderer.draw_calls = realloc(context->renderer.draw_calls, sizeof(void*) * context->renderer.draw_calls_capacity);
        if (!context->renderer.draw_calls) {
            BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to reallocate renderer draw calls array");
            return BLV_ERROR;
        }
    }    

    context->renderer.draw_calls[context->renderer.draw_calls_index] = data;
    context->renderer.draw_calls_index++;

    return BLV_OK;
}

BLV_Result blvRendererDrawFrame(blvContext *context) {

    vkWaitForFences(context->device.logical_device, 1, &context->renderer.in_flight_fence[context->renderer.frame_index], VK_TRUE, UINT64_MAX);
    vkResetFences(context->device.logical_device, 1, &context->renderer.in_flight_fence[context->renderer.frame_index]);

    // getting next image from swapchain
    uint32_t image_index;
    vkAcquireNextImageKHR(context->device.logical_device, context->swapchain.swapchain, UINT64_MAX,
                          context->renderer.image_available[context->renderer.frame_index], VK_NULL_HANDLE, &image_index);

    // Check Image Index
    if (context->renderer.images_in_flight[image_index] != VK_NULL_HANDLE) {
        vkWaitForFences(context->device.logical_device, 1, &context->renderer.images_in_flight[image_index], VK_TRUE, UINT64_MAX);
    }
    context->renderer.images_in_flight[image_index] = context->renderer.in_flight_fence[context->renderer.frame_index];

    // TODO: check image index before for saftey
    vkResetCommandBuffer(context->command_pool.buffers[context->renderer.frame_index], 0);
    blvCommandBufferRecord(context, context->renderer.frame_index, image_index);

    VkSemaphore wait_semaphores[] = {
        context->renderer.image_available[context->renderer.frame_index],
    };

    VkPipelineStageFlags wait_stages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    };

    VkSemaphore signal_semaphore[] = {
        context->renderer.render_finished[context->renderer.frame_index],
    };

    // TODO: make the code modulare with the counts and shit
    VkSubmitInfo submit_info = {0};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &context->command_pool.buffers[context->renderer.frame_index];
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphore;
    
    context->renderer.images_in_flight[image_index] = VK_NULL_HANDLE;

    if (vkQueueSubmit(context->graphics_queue.queue, 1, &submit_info, context->renderer.in_flight_fence[context->renderer.frame_index]) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_QUEUE_SUBMIT_ERROR, "Failed to submit graphics_queue");
        return BLV_ERROR;
    }

    VkPresentInfoKHR present_info = {0};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphore;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &context->swapchain.swapchain;
    present_info.pImageIndices = &image_index;

    if (vkQueuePresentKHR(context->graphics_queue.queue, &present_info) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_ERROR, "Failed to present queus");
        return BLV_ERROR;
    }

    context->renderer.frame_index = (context->renderer.frame_index + 1) % context->config.frames_in_flight;

    return BLV_OK;
}

BLV_Result blvRendererRenderQueue(blvContext *context, uint32_t index) {

    for (int32_t i = 0; i < context->renderer.draw_calls_index; i++) {
        if (context->renderer.draw_calls[i] == NULL) continue;
        switch (*((uint32_t*)(context->renderer.draw_calls[i]))) {
            case BLV_DRAW_TYPE_RECTANGLE:
                blvRectangleRender(context, index, (blvRectangle*)context->renderer.draw_calls[i]);
                break;
            default:
                break;
        }
        free((context->renderer.draw_calls[i]));
        context->renderer.draw_calls[i] = NULL;
    }
    context->renderer.draw_calls_index = 0;


    return BLV_OK;
}

void blvRendererDeinit(blvContext *context) {

    free(context->renderer.images_in_flight);
    context->renderer.images_in_flight = NULL;

    free(context->renderer.draw_calls);
    context->renderer.draw_calls = NULL;

    for (int32_t i = 0; i < context->config.frames_in_flight; i++) {
        vkDestroySemaphore(context->device.logical_device, context->renderer.image_available[i], NULL);
        vkDestroySemaphore(context->device.logical_device, context->renderer.render_finished[i], NULL);
        vkDestroyFence(context->device.logical_device, context->renderer.in_flight_fence[i], NULL);
    }

    free(context->renderer.image_available);
    context->renderer.image_available = NULL;

    free(context->renderer.render_finished);
    context->renderer.render_finished = NULL;

    free(context->renderer.in_flight_fence);
    context->renderer.in_flight_fence = NULL;
}

