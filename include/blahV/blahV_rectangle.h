
#ifndef BLAHV_RECTANGLE_H
#define BLAHV_RECTANGLE_H

#include <stdint.h>

#include "blahV/blahV_device.h"
#include "blahV_log.h"
#include "blahV_buffer.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct {
    uint32_t draw_type;
    float pos_x;
    float pos_y;
    float scale_x;
    float scale_y;
} blvRectangle;

extern float blv_rectangle_vertices[];
extern uint32_t blv_rectangle_indices[];
extern VkVertexInputBindingDescription blv_rectangle_binding_description;
extern VkVertexInputAttributeDescription blv_rectangle_attribute_description;

extern blvBuffer blv_rectangle_vertex_buffer;
extern blvBuffer blv_rectangle_indices_buffer;

BLV_Result blvRectangleInit(blvContext* context);
void blvRectangleDraw(blvContext* context, float pos_x, float pos_y, float scale_x, float scale_y);
void blvRectangleRender(blvContext* context, uint32_t index, blvRectangle* rect);
void blvRectangleDeinit(blvContext* context);

#endif
 
