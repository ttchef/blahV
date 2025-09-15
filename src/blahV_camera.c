
#include "blahV/renderer/blahV_camera.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_utils.h"
#include "blahV/vulkan/blahV_window.h"
#include "blahV/math/blahV_math.h"
#include <vulkan/vulkan_core.h>

blvCamera blvCameraInit(blvCameraCreateInfo *create_info) {
    if (!create_info) {
        BLV_LOG(BLV_LOG_DEBUG, "NULL passed to blvCameraInit function will use default parameters!\n");
        return (blvCamera){
            .speed = 5.0f,
            .sensitivity = 0.26f,
            .fov = 45.0f,
            .yaw = 0.0f,
            .pitch = 0.0f,
            .up = blvV3(0.0f, -1.0f, 0.0f),
            .position = blvV3(0.0f, 0.0f, 0.0f),
            .direction = blvV3(0.0f, 0.0f, -1.0f),
            .type = BLV_CAMERA_TYPE_SELF_MANAGED,
        };
    }

    blvCamera camera = {0};
    if (BLV_IS_ZERO(create_info->speed)) camera.speed = 5.0f;
    else camera.speed = create_info->speed;

    camera.type = create_info->type;

    // TOOD: ...

    return camera;
}

BLV_Result blvCameraUpdate(blvContext* context, blvCamera* camera, double* delta_time) {
    if (!camera) {
        BLV_SET_ERROR(BLV_CAMERA_ERROR, "Passed NULL as a camera parameter");
        return BLV_ERROR;
    }

    if (camera->type == BLV_CAMERA_TYPE_SELF_MANAGED) {
        BLV_SET_ERROR(BLV_CAMERA_ERROR, "Camera type is self managed");
        return BLV_ERROR;
    }

    double dt = delta_time ? *delta_time : 1;

    if (camera->type == BLV_CAMERA_TYPE_FREE_CAM) {
        blvCameraUpdateFreeCam(context, camera, dt);

    }

    blvCameraSendMatrices(context, camera);

    return BLV_OK;
}

void blvCameraUpdateFreeCam(blvContext* context, blvCamera *camera, double delta_time) {
    if (blvWindowGetKeyState(context, BLV_KEY_W, BLV_KEY_STATE_PRESS)) {
        blvVec3 tmp = blvV3MulF(blvV3MulF(blvV3Norm(blvV3Mul(camera->direction, blvV3(1.0f, 0.0f, 1.0f))), (float)delta_time), camera->speed);
        camera->position = blvV3Add(camera->position, tmp);
    }
    if (blvWindowGetKeyState(context, BLV_KEY_S, BLV_KEY_STATE_PRESS)) {
        blvVec3 tmp = blvV3MulF(blvV3MulF(blvV3Norm(blvV3Mul(camera->direction, blvV3(1.0f, 0.0f, 1.0f))), (float)delta_time), camera->speed);
        camera->position = blvV3Sub(camera->position, tmp);
    }
    if (blvWindowGetKeyState(context, BLV_KEY_A, BLV_KEY_STATE_PRESS)) {
        blvVec3 tmp = blvV3MulF(blvV3MulF(blvV3Norm(blvV3Cross(camera->direction, camera->up)), (float)delta_time), camera->speed);
        camera->position = blvV3Add(camera->position, tmp);
    }
    if (blvWindowGetKeyState(context, BLV_KEY_A, BLV_KEY_STATE_PRESS)) {
        blvVec3 tmp = blvV3MulF(blvV3MulF(blvV3Norm(blvV3Cross(camera->direction, camera->up)), (float)delta_time), camera->speed);
        camera->position = blvV3Sub(camera->position, tmp);
    }
}

BLV_Result blvCameraSendMatrices(blvContext *context, blvCamera *camera) {

    blvMat4 view_proj_matrix = blvMat4Translate(blvV3(0.0f, 0.0f, 0.0f));

    void* mapped;
    vkMapMemory(context->device.logical_device, context->graphcis_pipeline.uniform_buffers[context->renderer.frame_index].memory, 0, sizeof(blvMat4), 0, &mapped);
    memcpy(mapped, &view_proj_matrix, sizeof(view_proj_matrix));
    vkUnmapMemory(context->device.logical_device, context->graphcis_pipeline.uniform_buffers[context->renderer.frame_index].memory);

}

