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

    // Default Conifg
    blvConfig blv_config = {0};
    blvConfigInit(&blv_context, &blv_config);

    blvWindowCreateInfo window_create_info = {800, 600, "Hello BLAHV"};
    if (blvWindowInit(&blv_context, &window_create_info) != BLV_OK) {
        fprintf(stderr, "Failed to create a blv window!\n");
        return -1;
    }

    if (blvVulkanInit(&blv_context) != BLV_OK) {
        fprintf(stderr, "Failed to init blv vulkan!\n");
        return -1;
    }

    blvCameraCreateInfo camera_create_info = { 
        .speed = 5.0f,
        .sensitivity = 0.05f,
        .near = 0.01f,
        .far = 1000.0f,
        .position = blvV3(0.0f, 0.0f, 0.0f),
        .direction = blvV3(0.0f, 0.0f, -1.0f),
        .proj_type = BLV_CAMERA_PROJECTION_TYPE_PERSPECTIVE,
        .type = BLV_CAMERA_TYPE_FREE_CAM,
    };

    blvCamera camera = blvCameraInit(&camera_create_info);

    double current_time = blvWindowGetTime();
    double last_time = 0.0;

    while (!blvWindowShouldQuit(&blv_context)) {
        current_time = blvWindowGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        blvCameraUpdate(&blv_context, &camera, &delta_time);
        blvUpdate(&blv_context);

        blvRectangleDraw(&blv_context, -0.5f, -0.5f, 0.25f, 0.25f, blvV4(1.0f, 0.0f, 0.0f, 1.0f));
        blvRectangleDraw(&blv_context, 0.5f, -0.5f, 0.25f, 0.25f, blvV4(0.0f, 1.0f, 0.0f, 1.0f));
        blvRectangleDraw(&blv_context, -0.5f, 0.5f, 0.25f, 0.25f, blvV4(0.0f, 0.0f, 1.0f, 1.0f));
        blvRectangleDraw(&blv_context, 0.5f, 0.5f, 0.25f, 0.25f, blvV4(1.0f, 1.0f, 1.0f, 1.0f));


    }

    blvDeinit(&blv_context);

    return 0;
}
```

# Building

Vulkan And GLFW must be installed manually
For Handmande Math and stb_image
Clone Repo with submodules:

```bash

git clone --recurse-submodules https://github.com/ttchef/blahV.git

```

To build and run example: 

```bash

chmod +x run.sh
./run.sh

```

To build library 

```bash 

sudo make install BUILD=static
sudo make install BUILD=shared # For shared lib
```

# Dependencies
 - Vulkan (couldnt be guessed WOWOWOW)
 - glfw
 - Handmade Math (math library)
 - Stb_image (image loading)

# TODO (Plans for the next days)
 - Fix Config structure
 - Extend the Camera
 - Add Draw Cube or smth
 - Textures 
 - Organize c files in folders



