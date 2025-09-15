# Vulkan-Renderer

A simple vulkan renderer

# Example
```c

#include <stdio.h>
#include <blahV/blahV.h>

int main() {
    
    blvContext blv_context = {0};
    blvErrorSetLogLevel(BLV_LOG_DEBUG);
    blvErrorEnableValidationLayers(false);

    blvConfig blv_config = {0};
    blv_config.soft_resizing = true;

    blvWindowCreateInfo window_create_info = {800, 600, "Hello World"};
    if (blvWindowInit(&blv_context, &window_create_info, &blv_config) != BLV_OK) {
        fprintf(stderr, "Failed to create a blv window!\n");
        return -1;
    }

    if (blvVulkanInit(&blv_context) != BLV_OK) {
        fprintf(stderr, "Failed to init blv vulkan!\n");
        return -1;
    }

    while (!blvWindowShouldQuit(&blv_context)) {
        blvRectangleDraw(&blv_context, -0.5f, -0.5f, 0.25f, 0.25f, blvV4(1.0f, 0.0f, 0.0f, 1.0f));
        blvRectangleDraw(&blv_context, 0.5f, -0.5f, 0.25f, 0.25f, blvV4(0.0f, 1.0f, 0.0f, 1.0f));
        blvRectangleDraw(&blv_context, -0.5f, 0.5f, 0.25f, 0.25f, blvV4(0.0f, 0.0f, 1.0f, 1.0f));
        blvRectangleDraw(&blv_context, 0.5f, 0.5f, 0.25f, 0.25f, blvV4(1.0f, 1.0f, 1.0f, 1.0f));
        blvUpdate(&blv_context);
    }

    blvDeinit(&blv_context);

    return 0;
}

```

# Building

# Dependencies
 - Vulkan (couldnt be guessed WOWOWOW)
 - glfw
 - Handmade Math (math library)

# TODO
 - Fix Config structure
 - Implement Camera


