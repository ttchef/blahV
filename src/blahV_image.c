
#include "blahV/vulkan/blahV_image.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"
#include "blahV/core/blahV_utils.h"
#include <vulkan/vulkan_core.h>

void blvNothing() {

}

BLV_Result blvImageInit(blvContext *context, blvImage *image, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage) {

    // Image
    VkImageCreateInfo image_create_info = {0};
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.extent.width = width;
    image_create_info.extent.height = height;
    image_create_info.extent.depth = 1;
    image_create_info.mipLevels = 1;
    image_create_info.arrayLayers = 1;
    image_create_info.format = format;
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.usage = usage;
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(context->device.logical_device, &image_create_info, NULL, &image->image) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_IMAGE_ERROR, "Failed to create a vulkan image");
        return BLV_ERROR;
    }

    VkMemoryRequirements image_memory_requirements = {0};
    vkGetImageMemoryRequirements(context->device.logical_device, image->image, &image_memory_requirements);
    VkMemoryAllocateInfo image_allocate_info = {0};
    image_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    image_allocate_info.allocationSize = image_memory_requirements.size;
    image_allocate_info.memoryTypeIndex = blvFindMemoryType(context, image_memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    
    if (vkAllocateMemory(context->device.logical_device, &image_allocate_info, NULL, &image->memory) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_IMAGE_ERROR, "Failed to allocate vulkan image meory");
        return BLV_ERROR;
    }

    if (vkBindImageMemory(context->device.logical_device, image->image, image->memory, 0) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_IMAGE_ERROR, "Failed to bind vulkan image memory");
        return BLV_ERROR;
    }

    // Image View
    VkImageViewCreateInfo view_create_info = {0};
    view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_create_info.image = image->image;
    view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_create_info.format = format;
    view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_create_info.subresourceRange.levelCount = 1;
    view_create_info.subresourceRange.layerCount = 1;

    if (vkCreateImageView(context->device.logical_device, &view_create_info, NULL, &image->view) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_IMAGE_ERROR, "Failed to create vulkan image view");
        return BLV_ERROR;
    }


    return BLV_OK;
}

void blvImageDeinit(blvContext *context, blvImage *image) {
    vkDestroyImageView(context->device.logical_device, image->view, NULL);
    vkDestroyImage(context->device.logical_device, image->image, NULL);
    vkFreeMemory(context->device.logical_device, image->memory, NULL);
}

