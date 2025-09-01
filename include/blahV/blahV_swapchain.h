
#ifndef BLAHV_SWAPCHAIN_H
#define BLAHV_SWAPCHAIN_H

#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include <vulkan/vulkan_core.h>

typedef struct {
    VkImageUsageFlags usage;
} blvSwapchainCreateInfo;

typedef struct {
    VkSwapchainKHR swapchain;

    VkImage* images;
    VkImageView* image_views;
    uint32_t image_count;

    uint32_t width;
    uint32_t height;
    VkFormat format;
    VkExtent2D extent;
} blvSwapchain;

BLV_Result blvSwapchainInit(blvContext* context, blvSwapchainCreateInfo* create_info);
void blvSwapchainDeinit(blvContext* context);

#endif

