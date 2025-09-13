
#include "blahV/core/blahV_config.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"

BLV_Result blvConfigInit(blvContext* context) {
    context->config.frames_in_flight = 3;
    context->config.soft_resizing = true;

    return BLV_OK;
}
