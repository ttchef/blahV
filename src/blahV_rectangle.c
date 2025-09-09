
#include "blahV/blahV_rectangle.h"
#include "blahV/blahV_buffer.h"
#include "blahV/blahV_context.h"
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_renderer.h"
#include "blahV/blahV_utils.h"

#include <stdlib.h>
#include <vulkan/vulkan_core.h>

float blv_rectangle_vertices[] = {
    0.5f, 0.5f, 0.0f, 
    -0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

uint32_t blv_rectangle_indices[] = {
    0, 1, 2,
    1, 2, 3,
};

VkVertexInputBindingDescription blv_rectangle_binding_description = {
    .binding = 0,
    .stride = sizeof(float) * 3,
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
};

VkVertexInputAttributeDescription blv_rectangle_attribute_description = {
    .binding = 0,
    .location = 0,
    .format = VK_FORMAT_R32G32B32_SFLOAT,
};

blvBuffer blv_rectangle_vertex_buffer;
blvBuffer blv_rectangle_indices_buffer;

BLV_Result blvRectangleInit(blvContext *context) {
    
    blvBufferVertexCreate(context, &blv_rectangle_vertex_buffer, (void*)blv_rectangle_vertices, BLV_ARRAY_COUNT(blv_rectangle_vertices));
    blvBufferIndexCreate(context, &blv_rectangle_indices_buffer, (void*)blv_rectangle_indices, BLV_ARRAY_COUNT(blv_rectangle_indices));

    return BLV_OK;
}

void blvRectangleDraw(blvContext* context, int32_t pos_x, int32_t pos_y, uint32_t width, uint32_t height) {

    blvRectangle* rect = malloc(sizeof(blvRectangle));
    if (!rect) {
        BLV_SET_ERROR(BLV_ALLOC_FAIL, "Failed to allocate rectangle");
    }
    rect->draw_type = BLV_DRAW_TYPE_RECTANGLE;
    rect->pos_x = pos_x;
    rect->pos_y = pos_y;
    rect->width = width;
    rect->height = height;

    blvRendererPushDrawCall(context, rect);

}

void blvRectangleRender(blvContext *context, uint32_t index) {
    VkBuffer vertex_buffers[] = {blv_rectangle_vertex_buffer.buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(context->command_pool.buffers[index], 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(context->command_pool.buffers[index], blv_rectangle_indices_buffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(context->command_pool.buffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, context->graphcis_pipeline.layout, 0, 1,
                            &context->graphcis_pipeline.descriptor_sets[index], 0, NULL);
    vkCmdDrawIndexed(context->command_pool.buffers[index], BLV_ARRAY_COUNT(blv_rectangle_indices), 1, 0, 0, 0);
}

void blvRectangleDeinit(blvContext* context) {
    blvBufferDeinit(context, &blv_rectangle_vertex_buffer);
    blvBufferDeinit(context, &blv_rectangle_indices_buffer);
}

