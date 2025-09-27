
#ifndef BLAHV_UNIFORM_BUFFER_MANAGER_H
#define BLAHV_UNIFORM_BUFFER_MANAGER_H

#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_buffer.h"
#include "blahV/vulkan/blahV_device.h"

typedef struct {
    size_t count;
    blvBuffer* ubos;
} blvUniformBufferManager;

BLV_Result blvUinformBufferManagerInit(blvContext* context);
void blvUniformBufferManagerDeinit(blvContext* context);

#endif

