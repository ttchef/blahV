
#include "blahV/vulkan/blahV_pipeline_manager.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"
#include "blahV/vulkan/blahV_pipeline.h"
#include "blahV/renderer/blahV_rectangle.h"

BLV_Result blvPipelineManagerInit(blvContext *context) {

    // Right now only 2 different pipelines
    context->pipeline_manager.pipeline_count = 2;
    context->pipeline_manager.pipleines = malloc(sizeof(blvPipeline) * context->pipeline_manager.pipeline_count);
    if (!context->pipeline_manager.pipleines) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to allocate graphcis pipelines in pipeline manager");
        return BLV_ERROR;
    }

    // Default Pipeline 
    if (blvPipelineInit(context, "shaders/spv/default_vert.glsl", "shaders/spv/default_frag.glsl", 
        blv_rectangle_binding_description, blv_rectangle_attribute_description) != BLV_OK) {
        BLV_SET_ERROR(BLV_VULKAN_PIPELINE_ERROR, "Failed to create default rendering pipeline in pipeline manager");
        return BLV_ERROR;
    }

    

    return BLV_OK;
}

void blvPipelineManagerDeinit(blvContext *context) {
    
}

