
#include "blahV/blahV_log.h"
#include <stdio.h>

#include <blahV/blahV.h>

int main() {
    
    blvContext blv_context = {0};
    blvErrorSetLogLevel(BLV_LOG_DEBUG);
    blvErrorEnableValidationLayers(false);

    blvWindowCreateInfo window_create_info = {800, 600, "Hello World"};
    if (blvWindowInit(&blv_context, &window_create_info) != BLV_OK) {
        fprintf(stderr, "Failed to create a blv window!\n");
        return -1;
    }

    if (blvVulkanInit(&blv_context) != BLV_OK) {
        fprintf(stderr, "Failed to init blv vulkan!\n");
        return -1;
    }

    while (!blvWindowShouldQuit(&blv_context)) {
        blvWindowPollEvents();;
    }

    blvDeinit(&blv_context);

    return 0;
}

