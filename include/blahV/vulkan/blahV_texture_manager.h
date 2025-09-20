
#ifndef BLAHV_TEXTURE_2D_MANAGER_H
#define BLAHV_TEXTURE_2D_MANAGER_H

#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_device.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct {
    VkDescriptorPool descriptor_pool;
    VkDescriptorSetLayout descriptor_layout;
    VkDescriptorSet* desccriptor_sets;
} blvTextureManager;

BLV_Result blvTextureManagerInit(blvContext* context);
void blvTextureManagerDeinit(blvContext* context);

#endif
