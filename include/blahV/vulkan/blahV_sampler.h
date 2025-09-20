
#ifndef BLAHV_SAMPLER_H
#define BLAHV_SAMPLER_H

#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_device.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

// Sampler definies
typedef enum {
    BLV_FILTER_NEAREST = VK_FILTER_NEAREST,
    BLV_FILTER_LINEAR = VK_FILTER_LINEAR,
} blvSamplerFilter;

typedef enum {
    BLV_SAMPLER_MIPMAP_MODE_NEAREST = VK_SAMPLER_MIPMAP_MODE_NEAREST, 
    BLV_SAMPLER_MIPMAP_MODE_LINEAR = VK_SAMPLER_MIPMAP_MODE_LINEAR,
} blvSamplerMipmapMode;

typedef enum {
    BLV_SAMPLER_ADDRESS_MODE_REPEAR = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    BLV_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAR = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    BLV_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    BLV_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    BLV_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
} blvSamplerAddressMode;

typedef struct {
    blvSamplerFilter mag_filter;
    blvSamplerFilter min_filter;
    blvSamplerMipmapMode mipmap_mode;
    blvSamplerAddressMode address_mode;
    float mip_load_bias;
    float max_Anisotropy;
    float min_lod;
    float max_lod; 
} blvSamplerCreateInfo;

typedef struct {
    VkSampler sampler;   
} blvSampler;

blvSampler blvSamplerInit(blvContext* context, blvSamplerCreateInfo* create_info);
void blvSamplerDeinit(blvContext* context, blvSampler* sampler);

#endif
