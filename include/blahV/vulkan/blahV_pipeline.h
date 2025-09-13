
#ifndef BLAHV_PIPELINE_H
#define BLAHV_PIPELINE_H

#include "blahV/vulkan/blahV_device.h"
#include "blahV/core/blahV_log.h"
#include "blahV/vulkan/blahV_buffer.h"
#include <vulkan/vulkan_core.h>

typedef enum {
    BLV_SHADER_TYPE_VERTEX,
    BLV_SHADER_TYPE_FRAGMENT,
} BLV_Shader_Type;

typedef struct {
    VkPipeline pipeline;
    VkPipelineLayout layout;
    VkShaderModule vertex_shader;
    VkShaderModule fragment_shader;

    VkDescriptorPool descriptor_pool;
    VkDescriptorSetLayout descriptor_layout;
    VkDescriptorSet* descriptor_sets;
    blvBuffer* uniform_buffers;
} blvPipeline;

typedef struct {
    const char* filepath;
    BLV_Shader_Type shader_type;
} blvShaderCreateInfo;

typedef struct {
    char* data;
    uint32_t size;
    const char* filepath;
    BLV_Shader_Type shader_type;
} blvShader;

VkShaderModule* blvShaderInit(blvContext* context, blvShaderCreateInfo* create_info);

BLV_Result blvPipelineInit(blvContext* context, VkVertexInputBindingDescription binding_description,
                           VkVertexInputAttributeDescription attribute_description);
void blvPipelineDeinit(blvContext* context);


#endif
