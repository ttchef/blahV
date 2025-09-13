
#ifndef BLAHV_SURFACE_H
#define BLAHV_SURFACE_H

#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include <vulkan/vulkan_core.h>

typedef struct {
    VkSurfaceKHR surface;
} blvSurface;

BLV_Result blvSurfaceInit(blvContext* context);
void blvSurfaceDeinit(blvContext* context);

#endif
