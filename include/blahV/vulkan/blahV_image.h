
#ifndef BLAHV_IMAGE_H
#define BLAHV_IMAGE_H

#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_device.h"
#include <vulkan/vulkan.h>

// STB IMAGE 
#include "stb/stb_image.h"

typedef struct {
    VkImage image;
    VkImageView view;
    VkDeviceMemory memory;
} blvImage;

// TODO: temp BLVAPI 
BLVAPI BLV_Result blvImageInit(blvContext* context, blvImage* image, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage);
void blvImageDeinit(blvContext* context, blvImage* image);

// TODO: temp BLVAPI 
BLVAPI uint8_t* blvImageLoadStb(const char* path, int32_t* width, int32_t* height, int32_t* channels, int32_t desired_channels);

#endif
