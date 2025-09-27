
#ifndef BLAHV_PIPELINE_MANAGER_H
#define BLAHV_PIPELINE_MANAGER_H

#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_pipeline.h"

typedef struct {
    blvPipeline* pipleines;
    uint32_t pipeline_count;
} blvPipelineManager;

BLV_Result blvPipelineManagerInit(blvContext* context);
void blvPipelineManagerDeinit(blvContext* context);

#endif
