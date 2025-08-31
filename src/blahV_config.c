
#include "blahV/blahV_config.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_context.h"

BLV_Result blvConfigInit(blvContext* context) {
    context->config.frames_in_flight = 3;

    return BLV_OK;
}
