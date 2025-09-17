
#ifndef BLAHV_CONFIG_H
#define BLAHV_CONFIG_H

#include "blahV/vulkan/blahV_device.h"
#include "blahV/core/blahV_log.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t frames_in_flight;
    bool soft_resizing;
} blvConfig;

BLVAPI BLV_Result blvConfigInit(blvContext* context, blvConfig* config);

#endif

