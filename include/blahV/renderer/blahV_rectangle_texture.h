
#ifndef BLAHV_RECTANGLE_TEXTURE_H
#define BLAHV_RECTANGLE_TEXTURE_H

#include "blahV/math/blahV_math.h"
#include "blahV/vulkan/blahV_buffer.h"
#include <vulkan/vulkan.h>

typedef struct {
    uint32_t draw_type;
    float pos_x;
    float pos_y;
    float scale_x;
    float scale_y;
    blvVec4 color;
} blvRectangleTexture;

extern float blv_rectangle_texture_vertices[];
extern uint32_t blv_rectangle_texture_indices[];
extern VkVertexInputBindingDescription blv_rectangle_texture_binding_description;
extern VkVertexInputAttributeDescription blv_rectangle_texture_attribute_description[];

extern blvBuffer blv_rectangle_texture_vertex_buffer;
extern blvBuffer blv_rectangle_texture_indices_buffer;

BLV_Result blvRectangleTextureInit(blvContext* context);
BLVAPI void blvRectangleTextureDraw(blvContext* context, float pos_x, float pos_y, float scale_x, float scale_y, blvVec4 color);
void blvRectangleTextureRender(blvContext* context, uint32_t index, blvRectangleTexture* rect);
void blvRectangleTextureDeinit(blvContext* context);

#endif
