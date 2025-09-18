
#ifndef BLAHV_IMAGE_H
#define BLAHV_IMAGE_H

#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_device.h"
#include <vulkan/vulkan.h>

typedef struct {
    VkImage image;
    VkImageView view;
    VkDeviceMemory memory;
} blvImage;

BLVAPI BLV_Result blvImageInit(blvContext* context, blvImage* image, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage);
void blvImageDeinit(blvContext* context, blvImage* image);

#endif
