
#include "blahV/vulkan/blahV_texture_manager.h"
#include "blahV/core/blahV_context.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

BLV_Result blvTextureManagerInit(blvContext *context) {

    // Init texture index 
    context->texture_manager.current_texture_index = 0;
        
    VkDescriptorPoolSize pool_sizes[] = {
        (VkDescriptorPoolSize){
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = BLV_TEXTURE_MANAGER_MAX_DESCRIPTOR_SETS, // TODO: for pbr textures
        }
    };

    VkDescriptorPoolCreateInfo pool_create_info = {0};
    pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_create_info.maxSets = BLV_TEXTURE_MANAGER_MAX_DESCRIPTOR_SETS;
    pool_create_info.pPoolSizes = pool_sizes;
    pool_create_info.poolSizeCount = BLV_ARRAY_COUNT(pool_sizes);

    if (vkCreateDescriptorPool(context->device.logical_device, &pool_create_info, NULL, &context->texture_manager.descriptor_pool) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_DESCRIPTOR_SET_ERROR, "Failed to create descriptor pool for texture manager");
        return BLV_ERROR;
    }

    VkDescriptorSetLayoutBinding layout_bindings[] = {
        (VkDescriptorSetLayoutBinding){
            .binding = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = 0,
        }
    };

    VkDescriptorSetLayoutCreateInfo layout_create_info = {0};
    layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_create_info.bindingCount = BLV_ARRAY_COUNT(layout_bindings);
    layout_create_info.pBindings = layout_bindings;

    if (vkCreateDescriptorSetLayout(context->device.logical_device, &layout_create_info, NULL, &context->texture_manager.descriptor_layout) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_DESCRIPTOR_SET_ERROR, "Failed to create descriptor layout for texture manager");
        return BLV_ERROR;
    }

    context->texture_manager.desccriptor_sets = malloc(sizeof(VkDescriptorSet) * BLV_TEXTURE_MANAGER_MAX_DESCRIPTOR_SETS);
    if (!context->texture_manager.desccriptor_sets) {
         BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to malloc the descriptor sets for texture manager");
         return BLV_ERROR;
    }

    for (int32_t i = 0; i < BLV_TEXTURE_MANAGER_MAX_DESCRIPTOR_SETS; i++) {
        VkDescriptorSetAllocateInfo alloc_info = {0};
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.descriptorPool = context->texture_manager.descriptor_pool;
        alloc_info.descriptorSetCount = 1;
        alloc_info.pSetLayouts = &context->texture_manager.descriptor_layout;

        if (vkAllocateDescriptorSets(context->device.logical_device, &alloc_info, context->texture_manager.desccriptor_sets) != VK_SUCCESS) {
            BLV_SET_ERROR(BLV_VULKAN_DESCRIPTOR_SET_ERROR, "Failed to allocate vulkan descriptor sets for texture manager");
            return BLV_ERROR;
        }
    }

    return BLV_OK;
}

void blvTextureManagerDeinit(blvContext *context) {
    vkDestroyDescriptorPool(context->device.logical_device, context->texture_manager.descriptor_pool, NULL);
    vkDestroyDescriptorSetLayout(context->device.logical_device, context->texture_manager.descriptor_layout, NULL);
}

int32_t blvTextureManagerAddTexture(blvContext *context) {
    if (context->texture_manager.current_texture_index + 1 < BLV_TEXTURE_MANAGER_MAX_DESCRIPTOR_SETS) {
        uint32_t tmp = context->texture_manager.current_texture_index;
        context->texture_manager.current_texture_index++;
        return tmp;
    }
    
    return -1;
}

