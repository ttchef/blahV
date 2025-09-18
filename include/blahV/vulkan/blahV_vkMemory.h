
#ifndef BLAHV_VK_MEMORY_H
#define BLAHV_VK_MEMORY_H

#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_buffer.h"
#include "blahV/vulkan/blahV_device.h"

// With Staging Buffer
BLV_Result blvMemoryUploadDataToBuffer(blvContext* context, blvBuffer* buffer, void* data, uint32_t data_count);
BLV_Result blvMemoryUploadDataToBufferHostVisible(blvContext* context, blvBuffer* buffer, void* data, uint32_t data_count);

#endif
