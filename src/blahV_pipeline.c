
#include "blahV/blahV_pipeline.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_context.h"
#include "blahV/blahV_utils.h"
#include "blahV/blahV_buffer.h"
#include "blahV/blahV_math.h"
#include "blahV/blahV_push_constants.h"

#include <complex.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

VkShaderModule* blvShaderInit(blvContext *context, blvShaderCreateInfo *create_info) {

    FILE* file = fopen(create_info->filepath, "rb");
    if (!file) {
        BLV_SET_ERROR(BLV_FILE_ERROR, "Failed to open file: %s", create_info->filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // works because one spv command is 4 bytes so every file should be a multiple of 4
    // if not we know something isnt right here
    if ((file_size & 0x03) != 0) {
        BLV_SET_ERROR(BLV_VULKAN_PIPELINE_ERROR, "Shader Command is not 4 Bytes long");
        return NULL;
    }

    uint8_t buffer[file_size];
    fread(buffer, 1, file_size, file);

    VkShaderModuleCreateInfo shader_create_info = {0};
    shader_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_create_info.codeSize = file_size;
    shader_create_info.pCode = (uint32_t*)buffer;

    VkShaderModule* module = (create_info->shader_type == BLV_SHADER_TYPE_VERTEX) ? &context->graphcis_pipeline.vertex_shader : &context->graphcis_pipeline.fragment_shader;
    if (vkCreateShaderModule(context->device.logical_device, &shader_create_info, NULL, module) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_PIPELINE_ERROR, "Failed to create shader module");
        return NULL;
    }

    fclose(file);

    return module;
}

BLV_Result blvPipelineInit(blvContext *context, VkVertexInputBindingDescription binding_description,
                           VkVertexInputAttributeDescription attribute_description) {

    blvShaderCreateInfo vertex_shader_info = {0};
    vertex_shader_info.shader_type = BLV_SHADER_TYPE_VERTEX;
    vertex_shader_info.filepath = "shaders/spv/default_vert.spv";
    VkShaderModule* vertex_module = blvShaderInit(context, &vertex_shader_info);

    blvShaderCreateInfo fragment_shader_info = {0};
    fragment_shader_info.shader_type = BLV_SHADER_TYPE_FRAGMENT;
    fragment_shader_info.filepath = "shaders/spv/default_frag.spv";
    VkShaderModule* fragment_module = blvShaderInit(context, &fragment_shader_info);

    VkPipelineShaderStageCreateInfo shader_stages[2];
    shader_stages[0] = (VkPipelineShaderStageCreateInfo){0};
    shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shader_stages[0].module = context->graphcis_pipeline.vertex_shader;
    shader_stages[0].pName = "main";
 
    shader_stages[1] = (VkPipelineShaderStageCreateInfo){0};
    shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader_stages[1].module = context->graphcis_pipeline.fragment_shader;
    shader_stages[1].pName = "main";

    VkPipelineVertexInputStateCreateInfo vertex_input_state = {0};
    vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_state.vertexBindingDescriptionCount = 1;
    vertex_input_state.pVertexBindingDescriptions = &binding_description;
    vertex_input_state.vertexAttributeDescriptionCount = 1;
    vertex_input_state.pVertexAttributeDescriptions = &attribute_description;

    VkPipelineInputAssemblyStateCreateInfo assembly_input_state = {0};
    assembly_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    assembly_input_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewport_state = {0};
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterization_state = {0};
    rasterization_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_state.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisample_state = {0};
    multisample_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depth_stenci_state = {0};
    depth_stenci_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stenci_state.depthTestEnable = VK_TRUE;
    depth_stenci_state.depthWriteEnable = VK_TRUE;
    depth_stenci_state.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
    depth_stenci_state.minDepthBounds = 0.0f;
    depth_stenci_state.maxDepthBounds = 1.0f;

    VkPipelineColorBlendAttachmentState color_blend_attachment = {0};
    color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable = VK_TRUE;
    color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
    color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo color_blend_state = {0};
    color_blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state.attachmentCount = 1;
    color_blend_state.pAttachments = &color_blend_attachment;

    VkPipelineDynamicStateCreateInfo dynamic_state = {0};
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    VkDynamicState dynamic_states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    dynamic_state.pDynamicStates = dynamic_states;
    dynamic_state.dynamicStateCount = BLV_ARRAY_COUNT(dynamic_states);

    // ======= Descriptor sets  =======
    context->graphcis_pipeline.descriptor_sets = malloc(sizeof(VkDescriptorSet) * context->config.frames_in_flight);
    if (!context->graphcis_pipeline.descriptor_sets) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to allocate pipeline descriptor sets");
        return BLV_ERROR;
    }

    VkDescriptorPoolSize descriptor_pool_size = {0};
    descriptor_pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_pool_size.descriptorCount = context->config.frames_in_flight;

    VkDescriptorPoolCreateInfo descriptor_pool_create_info = {0};
    descriptor_pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool_create_info.poolSizeCount = 1;
    descriptor_pool_create_info.pPoolSizes = &descriptor_pool_size;
    descriptor_pool_create_info.maxSets = context->config.frames_in_flight;

    if (vkCreateDescriptorPool(context->device.logical_device, &descriptor_pool_create_info, NULL, &context->graphcis_pipeline.descriptor_pool) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_DESCRIPTOR_SET_ERROR, "Failed to create Descriptor pool");
        return BLV_ERROR;
    }

    VkDescriptorSetLayoutBinding descriptor_set_layout_binding = {0};
    descriptor_set_layout_binding.binding = 0;
    descriptor_set_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_set_layout_binding.descriptorCount = 1;
    descriptor_set_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo descriptor_Set_layout_create_info = {0};
    descriptor_Set_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_Set_layout_create_info.bindingCount = 1;
    descriptor_Set_layout_create_info.pBindings = &descriptor_set_layout_binding;

    if (vkCreateDescriptorSetLayout(context->device.logical_device, &descriptor_Set_layout_create_info, NULL, &context->graphcis_pipeline.descriptor_layout) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_DESCRIPTOR_SET_ERROR, "Failed to create Descriptor set layout");
        return BLV_ERROR;
    }

    // Uniform Buffers
    context->graphcis_pipeline.uniform_buffers = malloc(sizeof(blvBuffer) * context->config.frames_in_flight);
    if (!context->graphcis_pipeline.uniform_buffers) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to allocate pipeline uniform buffers");
        return BLV_ERROR;
    }

    for (int32_t i = 0; i < context->config.frames_in_flight; i++) {
        blvBufferCreate(context, sizeof(blvMat4), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
                        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &context->graphcis_pipeline.uniform_buffers[i]);
    }

    // Allocation of the sets 
    for (int32_t i = 0; i < context->config.frames_in_flight; i++) {
        VkDescriptorSetAllocateInfo descriptor_set_alloc_info = {0};
        descriptor_set_alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptor_set_alloc_info.descriptorPool = context->graphcis_pipeline.descriptor_pool;
        descriptor_set_alloc_info.descriptorSetCount = 1;
        descriptor_set_alloc_info.pSetLayouts = &context->graphcis_pipeline.descriptor_layout;

        if (vkAllocateDescriptorSets(context->device.logical_device, &descriptor_set_alloc_info, &context->graphcis_pipeline.descriptor_sets[i]) != VK_SUCCESS) {
            BLV_SET_ERROR(BLV_VULKAN_DESCRIPTOR_SET_ERROR, "Failed to vulkan allocate Descriptor sets");
            return BLV_ERROR;
        }

        VkDescriptorBufferInfo descriptor_buffer_info = {0};
        descriptor_buffer_info.buffer = context->graphcis_pipeline.uniform_buffers[i].buffer;
        descriptor_buffer_info.offset = 0;
        descriptor_buffer_info.range = sizeof(blvMat4);

        VkWriteDescriptorSet descriptor_set_write = {0};
        descriptor_set_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_set_write.dstSet = context->graphcis_pipeline.descriptor_sets[i];
        descriptor_set_write.dstBinding = 0;
        descriptor_set_write.dstArrayElement = 0;
        descriptor_set_write.descriptorCount = 1;
        descriptor_set_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_set_write.pBufferInfo = &descriptor_buffer_info;

        vkUpdateDescriptorSets(context->device.logical_device, 1, &descriptor_set_write, 0, NULL);
    }

    // Push Constans
    VkPushConstantRange push_constant_range = {0};
    push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    push_constant_range.offset = 0;
    push_constant_range.size = sizeof(blvModelColorPushConstant);

    VkPipelineLayoutCreateInfo layout_info = {0};
    layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.setLayoutCount = 1;
    layout_info.pSetLayouts = &context->graphcis_pipeline.descriptor_layout;
    layout_info.pushConstantRangeCount = 1;
    layout_info.pPushConstantRanges = &push_constant_range;
    
    if (vkCreatePipelineLayout(context->device.logical_device, &layout_info, NULL, &context->graphcis_pipeline.layout) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_PIPELINE_ERROR, "Failed to create pipeline layout");
        return BLV_ERROR;
    }

    // ====== End Descriptor Sets =======

    // Dynamic Rendering Info 
    VkPipelineRenderingCreateInfoKHR pipeline_dynamic_rendering_info = {0};
    pipeline_dynamic_rendering_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    pipeline_dynamic_rendering_info.colorAttachmentCount = 1;
    pipeline_dynamic_rendering_info.pColorAttachmentFormats = &context->swapchain.format;
    pipeline_dynamic_rendering_info.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;
    pipeline_dynamic_rendering_info.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

    VkGraphicsPipelineCreateInfo pipeline_create_info = {0};
    pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_create_info.pNext = &pipeline_dynamic_rendering_info; // dynamic rendering
    pipeline_create_info.subpass = 0;
    pipeline_create_info.stageCount = BLV_ARRAY_COUNT(shader_stages);
    pipeline_create_info.pStages = shader_stages;
    pipeline_create_info.pVertexInputState = &vertex_input_state;
    pipeline_create_info.pInputAssemblyState = &assembly_input_state;
    pipeline_create_info.pViewportState = &viewport_state;
    pipeline_create_info.pRasterizationState = &rasterization_state;
    pipeline_create_info.pMultisampleState = &multisample_state;
    pipeline_create_info.pDepthStencilState = &depth_stenci_state;
    pipeline_create_info.pColorBlendState = &color_blend_state;
    pipeline_create_info.pDynamicState = &dynamic_state;
    pipeline_create_info.renderPass = VK_NULL_HANDLE;


    pipeline_create_info.layout = context->graphcis_pipeline.layout;
    if (vkCreateGraphicsPipelines(context->device.logical_device, 0, 1, &pipeline_create_info, NULL, &context->graphcis_pipeline.pipeline) != VK_SUCCESS) {
        BLV_SET_ERROR(BLV_VULKAN_PIPELINE_ERROR, "Failed to create graphcis pipeline");
        return BLV_ERROR;
    }

    // Delete modules
    vkDestroyShaderModule(context->device.logical_device, *vertex_module, NULL);
    vkDestroyShaderModule(context->device.logical_device, *fragment_module, NULL);

    return BLV_OK;
}

void blvPipelineDeinit(blvContext *context) {

    for (int32_t i = 0; i < context->config.frames_in_flight; i++) {
        blvBufferDeinit(context, &context->graphcis_pipeline.uniform_buffers[i]);
    }

    vkDestroyDescriptorPool(context->device.logical_device, context->graphcis_pipeline.descriptor_pool, NULL);
    vkDestroyDescriptorSetLayout(context->device.logical_device, context->graphcis_pipeline.descriptor_layout, NULL);
    vkDestroyPipelineLayout(context->device.logical_device, context->graphcis_pipeline.layout, NULL);
    vkDestroyPipeline(context->device.logical_device, context->graphcis_pipeline.pipeline, NULL);

    free(context->graphcis_pipeline.descriptor_sets);
    context->graphcis_pipeline.descriptor_sets = NULL;
    
    free(context->graphcis_pipeline.uniform_buffers);
    context->graphcis_pipeline.uniform_buffers = NULL;
}

