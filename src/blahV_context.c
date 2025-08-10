
#include "blahV/blahV_context.h"
#include "blahV/blahV_log.h"
#include "blahV/blahV_window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

void blvDeinit(blvContext *context) {
    


    blvWindowDeinit(context);
}

