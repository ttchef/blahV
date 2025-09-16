
#include "blahV/renderer/blahV_camera.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_utils.h"
#include "blahV/vulkan/blahV_window.h"
#include "blahV/math/blahV_math.h"
#include <GLFW/glfw3.h>
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
            .near = 0.01f,
            .far = 100.0f,
            .first_mouse = true,
            .last_mouse_pos_x = 0.0, 
            .last_mouse_pos_y = 0.0,
            .up = blvV3(0.0f, -1.0f, 0.0f),
            .position = blvV3(0.0f, 0.0f, 0.0f),
            .direction = blvV3(0.0f, 0.0f, -1.0f),
            .type = BLV_CAMERA_TYPE_SELF_MANAGED,
        };
    }

    blvCamera camera = {0};
    if (BLV_IS_ZERO(create_info->speed)) camera.speed = 5.0f;
    else camera.speed = create_info->speed;

    if (BLV_IS_ZERO(create_info->sensitivity)) camera.sensitivity = 0.26f;
    else camera.sensitivity = create_info->sensitivity;

    if (BLV_IS_ZERO(create_info->fov)) camera.fov = 45.0f;
    else camera.fov = create_info->fov;

    if (BLV_IS_ZERO(create_info->near)) camera.near = 0.01f;
    else camera.near = create_info->near;

    if (BLV_IS_ZERO(create_info->far)) camera.far = 100.0f;
    else camera.far = create_info->far;

    if (BLV_IS_ZERO(create_info->type)) camera.type = BLV_CAMERA_TYPE_SELF_MANAGED;
    else camera.type = create_info->type;

    camera.up = blvV3(0.0f, -1.0f, 0.0f);
    camera.position = blvV3(0.0f, 0.0f, -2.0f);
    camera.direction = blvV3(0.0f, 0.0f, 1.0f);


    // TOOD: ...

    return camera;
}

BLV_Result blvCameraUpdate(blvContext *context, blvCamera *camera, double *delta_time) {

    if (camera->type != BLV_CAMERA_TYPE_SELF_MANAGED) blvCameraUpdateBlvCams(context, camera, delta_time);
    blvCameraSendMatrices(context, camera);

    return BLV_OK;
}

BLV_Result blvCameraUpdateBlvCams(blvContext* context, blvCamera* camera, double* delta_time) {
    if (!camera) {
        BLV_SET_ERROR(BLV_CAMERA_ERROR, "Passed NULL as a camera parameter");
        return BLV_ERROR;
    }

    double dt = delta_time ? (*delta_time) : 1;

    if (camera->type == BLV_CAMERA_TYPE_FREE_CAM) {
        blvCameraUpdateFreeCam(context, camera, dt);

    }
  
    return BLV_OK;
}

void blvCameraUpdateFreeCam(blvContext* context, blvCamera *camera, double delta_time) {

    blvWindowSetInputMode(context, BLV_CURSOR, BLV_CURSOR_DISABLED);

    // Keyboard
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
    if (blvWindowGetKeyState(context, BLV_KEY_D, BLV_KEY_STATE_PRESS)) {
        blvVec3 tmp = blvV3MulF(blvV3MulF(blvV3Norm(blvV3Cross(camera->direction, camera->up)), (float)delta_time), camera->speed);
        camera->position = blvV3Sub(camera->position, tmp);
    }
    if (blvWindowGetKeyState(context, BLV_KEY_SPACE, BLV_KEY_STATE_PRESS)) {
        blvVec3 tmp = blvV3MulF(blvV3MulF(blvV3Norm(camera->up), (float)delta_time), camera->speed);
        camera->position = blvV3Sub(camera->position, tmp);
    }
    if (blvWindowGetKeyState(context, BLV_KEY_LEFT_SHIFT, BLV_KEY_STATE_PRESS)) {
        blvVec3 tmp = blvV3MulF(blvV3MulF(blvV3Norm(camera->up), (float)delta_time), camera->speed);
        camera->position = blvV3Add(camera->position, tmp);
    }
    if (blvWindowGetKeyState(context, BLV_KEY_T, BLV_KEY_STATE_PRESS)) {
        blvWindowSetInputMode(context, BLV_CURSOR, BLV_CURSOR_NORMAL);
    }
    
    // Mouse
    double pos_x, pos_y;
    blvWindowGetCursorPosition(context, &pos_x, &pos_y);
    double dx = pos_x - camera->last_mouse_pos_x;
    double dy = pos_y - camera->last_mouse_pos_y;

    // To make the cursor not exit the window
    blvWindowSetCursorPos(context, context->window.width / 2.0, context->window.height / 2.0);
    camera->last_mouse_pos_x = context->window.width / 2.0;
    camera->last_mouse_pos_y = context->window.height / 2.0;

    camera->yaw += (float)dx * camera->sensitivity;
    camera->pitch -= (float)dy * camera->sensitivity;
    
    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

    blvVec3 front;
    front.X = (float)blvMathCos(blvMathDegToRad((float)camera->pitch)) * (float)blvMathSin(blvMathDegToRad((float)camera->yaw));
    front.Y = (float)blvMathSin(blvMathDegToRad(camera->pitch));
    front.Z = (-(float)blvMathCos(blvMathDegToRad(camera->pitch))) * (float)blvMathCos(blvMathDegToRad(camera->yaw));
    camera->direction = blvV3Norm(front);

}

BLV_Result blvCameraSendMatrices(blvContext *context, blvCamera *camera) {

    blvMat4 view_matrix = blvMat4LookA_LH(camera->position, blvV3Add(camera->position, camera->direction), camera->up);
    blvMat4 proj_matrix = blvMat4Perspective_LH_ZO(camera->fov, context->window.width / context->window.height, camera->near, camera->far);
    blvMat4 view_proj_matrix = blvMat4Mul(proj_matrix, view_matrix);

    void* mapped;
    vkMapMemory(context->device.logical_device, context->graphcis_pipeline.uniform_buffers[context->renderer.frame_index].memory, 0, sizeof(blvMat4), 0, &mapped);
    memcpy(mapped, &view_proj_matrix, sizeof(view_proj_matrix));
    vkUnmapMemory(context->device.logical_device, context->graphcis_pipeline.uniform_buffers[context->renderer.frame_index].memory);

    return BLV_OK;

}

