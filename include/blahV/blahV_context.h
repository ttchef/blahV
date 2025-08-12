
#ifndef BLAHV_CONTEXT_H
#define BLAHV_CONTEXT_H

#include "blahV/blahV_log.h"
#include "blahV/blahV_queue.h"
#include "blahV_window.h"
#include "blahV_device.h"
#include "blahV_surface.h"

struct blvContext {
    blvWindow window;
    blvDevice device;
    blvQueue graphics_queue;
    blvSurface surface;
};

BLV_Result blvVulkanInit(blvContext* context);
void blvDeinit(blvContext* context);

#endif
