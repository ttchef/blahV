
#ifndef BLAHV_CONTEXT_H
#define BLAHV_CONTEXT_H

#include "blahV/blahV_log.h"
#include "blahV_window.h"
#include "blahV_device.h"

struct blvContext {
    blvWindow window;
    blvDevice device;
};

BLV_Result blvVulkanInit(blvContext* context);
void blvDeinit(blvContext* context);

#endif
