
#include "blahV/core/blahV_config.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"
#include "blahV/core/blahV_utils.h"

BLV_Result blvConfigInit(blvContext* context, blvConfig* config) {
    if(!context) {
        BLV_SET_ERROR(BLV_INVALID_FUNCTION_INPUT, "Context function argument is NULL that should be a valid pointer");
        return BLV_ERROR;
    }
    
    if (!config) {
        context->config = (blvConfig){
            .frames_in_flight = 3,
            .soft_resizing = true,
        };
    }
    if (BLV_IS_ZERO(config->frames_in_flight)) context->config.frames_in_flight = 3;
    else context->config.frames_in_flight = config->frames_in_flight;

    if (BLV_IS_ZERO(config->soft_resizing)) context->config.soft_resizing = true;
    else context->config.soft_resizing = config->soft_resizing;

    return BLV_OK;
}
