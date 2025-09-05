
#ifndef BLAHV_CONFIG_H
#define BLAHV_CONFIG_H

#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t frames_in_flight;
    bool soft_resizing;
} blvConfig;

BLV_Result blvConfigInit(blvContext* context);

#endif

