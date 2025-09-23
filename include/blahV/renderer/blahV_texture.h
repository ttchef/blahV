
#ifndef BLAHV_TEXTURE_H
#define BLAHV_TEXTURE_H

#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_device.h"
#include "blahV/vulkan/blahV_image.h"
#include "blahV/vulkan/blahV_sampler.h"
#include <vulkan/vulkan.h>

typedef struct {
    int32_t id;
    uint32_t width;
    uint32_t height;
    blvImage image;
} blvTexture2D;

BLVAPI BLV_Result blvTexture2DInit(blvContext* context, blvTexture2D* texture, const char* image_path, blvSampler* sampler);
BLVAPI void blvTexture2DDeinit(blvContext* context, blvTexture2D* texture);

#endif

