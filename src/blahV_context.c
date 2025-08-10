
#include "blahV/blahV_context.h"
#include "blahV/blahV_device.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_window.h"
#include <vulkan/vulkan_core.h>

BLV_Result blvVulkanInit(blvContext *context) {
    blvDeviceInit(context);
}

void blvDeinit(blvContext *context) {
    


    blvWindowDeinit(context);
}

