
#ifndef BLAHV_TEXTURE_2D_MANAGER_H
#define BLAHV_TEXTURE_2D_MANAGER_H

#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_device.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

// TODO: make a better thing than whatever this is
#define BLV_TEXTURE_MANAGER_MAX_DESCRIPTOR_SETS 100

typedef struct {
    VkDescriptorPool descriptor_pool;
    VkDescriptorSetLayout descriptor_layout;
    VkDescriptorSet* desccriptor_sets;

    uint32_t current_texture_index;
} blvTextureManager;

BLV_Result blvTextureManagerInit(blvContext* context);
void blvTextureManagerDeinit(blvContext* context);

// returns texture id (-1 meaning error)
int32_t blvTextureManagerAddTexture(blvContext* context);

#endif

