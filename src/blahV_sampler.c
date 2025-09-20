
#include "blahV/vulkan/blahV_sampler.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"
#include "blahV/core/blahV_utils.h"

#include <complex.h>
#include <vulkan/vulkan_core.h>

blvSampler blvSamplerInit(blvContext *context, blvSamplerCreateInfo* create_info) {
    blvSampler sampler = {0};

    VkSamplerCreateInfo sampler_create_info = {0};
    sampler_create_info.magFilter = VK_FILTER_NEAREST;
    sampler_create_info.minFilter = VK_FILTER_NEAREST;
    sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    sampler_create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    sampler_create_info.mipLodBias = 0.0f;
    sampler_create_info.maxAnisotropy = 1.0f;
    sampler_create_info.minLod = 0.0f;
    sampler_create_info.maxLod = 1.0f;

    // Default Values
    if (!create_info) {
        if (vkCreateSampler(context->device.logical_device, &sampler_create_info, NULL, &sampler.sampler) != VK_SUCCESS) {
            BLV_SET_ERROR(BLV_VULKAN_SAMPLER_ERROR, "Failed to create vulkan sampler");
            return (blvSampler){0};
        }
    }

    if (BLV_NOT_ZERO(create_info->mag_filter)) sampler_create_info.magFilter = create_info->mag_filter;
    if (BLV_NOT_ZERO(create_info->min_filter)) sampler_create_info.minFilter = create_info->min_filter;
    if (BLV_NOT_ZERO(create_info->mipmap_mode)) sampler_create_info.mipmapMode = create_info->mipmap_mode;

    if (BLV_NOT_ZERO(create_info->address_mode)) {
        sampler_create_info.addressModeU = create_info->address_mode;
        sampler_create_info.addressModeV = create_info->address_mode;
        sampler_create_info.addressModeW = create_info->address_mode;
    }

    if (BLV_NOT_ZERO(create_info->mip_load_bias)) sampler_create_info.mipLodBias = create_info->mip_load_bias;
    if (BLV_NOT_ZERO(create_info->max_Anisotropy)) sampler_create_info.maxAnisotropy = create_info->max_Anisotropy;
    if (BLV_NOT_ZERO(create_info->min_lod)) sampler_create_info.minLod = create_info->min_lod;
    if (BLV_NOT_ZERO(create_info->max_lod)) sampler_create_info.maxLod = create_info->max_lod;

    if (vkCreateSampler(context->device.logical_device, &sampler_create_info, NULL, &sampler.sampler) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_SAMPLER_ERROR, "Failed to create vulkan sampler");
        return (blvSampler){0};
    }

    return sampler;
}

void blvSamplerDeinit(blvContext *context, blvSampler* sampler) {
    vkDestroySampler(context->device.logical_device, sampler->sampler, NULL);
}


