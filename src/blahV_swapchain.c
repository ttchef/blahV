
#include "blahV/blahV_swapchain.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_context.h"

#include <stdlib.h>
#include <vulkan/vulkan_core.h>

BLV_Result blvSwapchainInit(blvContext *context, blvSwapchainCreateInfo* create_info) {
    
    blvSwapchainCreateInfo info = {0};

    if (!create_info) {
        BLV_LOG(BLV_LOG_WARNING, "Not specified swapchain create_info will use the default values!\n");
        info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        create_info = &info;
    }

    // support presenting?
    VkBool32 supports_present = 0;
    vkGetPhysicalDeviceSurfaceSupportKHR(context->device.physical_device, context->graphics_queue.family_index, context->surface.surface, &supports_present);
    if (!supports_present) {
        BLV_SET_ERROR(BLV_VULKAN_SWAPCHAIN_ERROR, "Graphics Queue does not support present mode");
        return BLV_ERROR;
    }

    // Get Image formats
    uint32_t num_formats = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(context->device.physical_device, context->surface.surface, &num_formats, NULL);
    if (num_formats <= 0) {
        BLV_SET_ERROR(BLV_VULKAN_SWAPCHAIN_ERROR, "No surface image formats available");
        return BLV_ERROR;
    }

    VkSurfaceFormatKHR available_formats[num_formats];
    vkGetPhysicalDeviceSurfaceFormatsKHR(context->device.physical_device, context->surface.surface, &num_formats, available_formats);

    VkFormat format = available_formats[0].format;
    VkColorSpaceKHR color_space = available_formats[0].colorSpace;

    VkSurfaceCapabilitiesKHR surface_capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->device.physical_device, context->surface.surface, &surface_capabilities);
    if (surface_capabilities.currentExtent.width == 0xFFFFFFFF) {
        int width, height;
        glfwGetFramebufferSize(context->window.glfw_window, &width, &height);

        surface_capabilities.currentExtent.width = width; // need to be improved for saftey in a decade
        surface_capabilities.currentExtent.height = height;
    }

    VkSwapchainCreateInfoKHR swapchain_create_info = {0};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.surface = context->surface.surface;
    swapchain_create_info.minImageCount = 3;
    swapchain_create_info.imageFormat = format;
    swapchain_create_info.imageColorSpace = color_space;
    swapchain_create_info.imageExtent = surface_capabilities.currentExtent;
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = create_info->usage;
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;

    if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        swapchain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else {
        swapchain_create_info.preTransform = surface_capabilities.currentTransform;
    }

    if (vkCreateSwapchainKHR(context->device.logical_device, &swapchain_create_info, NULL, &context->swapchain.swapchain) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_SWAPCHAIN_ERROR, "Failed to create swapchain");
        return BLV_ERROR;
    }

    context->swapchain.format = format;
    context->swapchain.width = surface_capabilities.currentExtent.width;
    context->swapchain.height = surface_capabilities.currentExtent.height;

    // Get Swapchain Images
    uint32_t num_images = 0;
    vkGetSwapchainImagesKHR(context->device.logical_device, context->swapchain.swapchain, &num_images, NULL);
    context->swapchain.image_count = num_images;

    context->swapchain.images = malloc(sizeof(VkImage) * num_images);
    if (!context->swapchain.images) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Couldnt allocate swapchain images");
        return BLV_ERROR;
    } 

    vkGetSwapchainImagesKHR(context->device.logical_device, context->swapchain.swapchain, &num_images, context->swapchain.images);

    context->swapchain.image_views = malloc(sizeof(VkImageView) * num_images);
    if (!context->swapchain.image_views) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Couldnt allocate swapchain image views");
        return BLV_ERROR;
    }

    for (uint32_t i = 0; i < num_images; i++) {
        VkImageViewCreateInfo create_info = {0};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = context->swapchain.images[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = format;
        create_info.components = (VkComponentMapping){0};
        create_info.subresourceRange = (VkImageSubresourceRange){ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

        if (vkCreateImageView(context->device.logical_device, &create_info, NULL, &context->swapchain.image_views[i]) != VK_SUCCESS) {
            BLV_SET_ERROR(BLV_VULKAN_SWAPCHAIN_ERROR, "Failed to create image views");
            return BLV_ERROR;
        }
    }

    return BLV_OK;
}

void blvSwapchainDeinit(blvContext *context) {
    if (context->swapchain.images) {
        free(context->swapchain.images);
        context->swapchain.images = NULL;
    }

    for (uint32_t i = 0; i < context->swapchain.image_count; i++) {
        vkDestroyImageView(context->device.logical_device, context->swapchain.image_views[i], NULL);
    }

    if (context->swapchain.image_views) {
        free(context->swapchain.image_views);
        context->swapchain.image_views = NULL;
    }

    vkDestroySwapchainKHR(context->device.logical_device, context->swapchain.swapchain, NULL);
}



