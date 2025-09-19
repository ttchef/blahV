
#ifndef BLAHV_VK_MEMORY_H
#define BLAHV_VK_MEMORY_H

#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_buffer.h"
#include "blahV/vulkan/blahV_device.h"
#include "blahV/vulkan/blahV_image.h"

// With Staging Buffer
BLV_Result blvMemoryUploadDataToBuffer(blvContext* context, blvBuffer* buffer, void* data, uint32_t data_count);
BLV_Result blvMemoryUploadDataToImage(blvContext* context, blvImage* image, void* data, uint32_t data_count, uint32_t width, uint32_t height,
                                      VkImageLayout final_layout, VkAccessFlags dst_access_mask);

// Memcpy
BLV_Result blvMemoryUploadDataToBufferHostVisible(blvContext* context, blvBuffer* buffer, void* data, uint32_t data_count);

#endif
