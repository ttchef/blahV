
#ifndef BLAHV_RECTANGLE_TEXTURE_H
#define BLAHV_RECTANGLE_TEXTURE_H

#include "blahV/core/blahV_log.h"
#include "blahV/renderer/blahV_texture.h"
#include "blahV/math/blahV_math.h"
#include "blahV/vulkan/blahV_device.h"

// IMPLEMENTATION in blahV_rectangle.c

typedef struct {
    uint32_t draw_type;
    float pos_x;
    float pos_y;
    float scale_x;
    float scale_y;
    blvVec4 color;
    blvTexture2D* texture;
} blvRectangleTexture;

BLVAPI void blvRectangleTextureDraw(blvContext* context, float pos_x, float pos_y, float scale_x, float scale_y, blvVec4 color, blvTexture2D* texture);
void blvRectangleTextureRender(blvContext* context, uint32_t index, blvRectangleTexture* rect);

#endif

