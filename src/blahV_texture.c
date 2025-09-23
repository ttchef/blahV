
#include "blahV/renderer/blahV_texture.h"
#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_image.h"
#include "blahV/vulkan/blahV_sampler.h"
#include "blahV/vulkan/blahV_vkMemory.h"
#include "blahV/vulkan/blahV_texture_manager.h"
#include "blahV/core/blahV_context.h"
#include <vulkan/vulkan_core.h>

BLV_Result blvTexture2DInit(blvContext *context, blvTexture2D *texture, const char *image_path, blvSampler* sampler) {

    int32_t width, height, channels;
    uint8_t* data = blvImageLoadStb(image_path, &width, &height, &channels, 4);
    if (!data) {
        BLV_SET_ERROR(BLV_IMAGE_ERROR, "Failed to load image: %s\n", image_path);
        return BLV_ERROR;
    }

    if (blvImageInit(context, &texture->image, (uint32_t)width, (uint32_t)height, VK_FORMAT_R8G8B8A8_UNORM, 
                     VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT) != BLV_OK) {
        BLV_SET_ERROR(BLV_IMAGE_ERROR, "Failed to create blvImage: %s\n", image_path);
        return BLV_ERROR;
    }

    if (blvMemoryUploadDataToImage(context, &texture->image, data, width * height * 4, width, height,
                               VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT) != BLV_OK) {
        BLV_SET_ERROR(BLV_VULKAN_MEMORY_ERROR, "Failed to upload data to image: %s\n", image_path);
        return BLV_ERROR;
    }

    texture->id = blvTextureManagerAddTexture(context);
    if (texture->id == -1) {
        BLV_LOG(BLV_LOG_WARNING, "Couldnt add texture because all texture slots are filled\n");
        return BLV_ERROR;
    }

    texture->width = (uint32_t)width;
    texture->height = (uint32_t)height;

    // Create Descriptor Set and put data there
    VkDescriptorImageInfo image_info = {
        .sampler = sampler ? sampler->sampler : context->sampler_default.sampler,
        .imageView = texture->image.view,
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };

    VkWriteDescriptorSet descriptor_write = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = context->texture_manager.desccriptor_sets[texture->id], 
        .dstBinding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .pImageInfo = &image_info,
    };
    
    vkUpdateDescriptorSets(context->device.logical_device, 1, &descriptor_write, 0, NULL);

    return BLV_OK;
}

void blvTexture2DDeinit(blvContext *context, blvTexture2D *texture) {
    blvImageDeinit(context, &texture->image);
}


