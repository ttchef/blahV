
#include <stdio.h>
#include <blahV/blahV.h>

int main() {
    
    blvContext blv_context = {0};
    blvErrorSetLogLevel(BLV_LOG_DEBUG);
    blvErrorEnableValidationLayers(true);

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

    int32_t width, height, channels;
    uint8_t* data = blvImageLoadStb("/home/ttchef/Downloads/ski-background.jpg", &width, &height, &channels, 4);
    blvImage image;
    blvImageInit(&blv_context, &image, width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
    blvMemoryUploadDataToImage(&blv_context, &image, data, width * height * 4, width, height, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);


    double current_time = blvWindowGetTime();
    double last_time = current_time;

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

