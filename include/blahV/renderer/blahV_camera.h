
#ifndef BLAHV_CAMERA_H
#define BLAHV_CAMERA_H

#include "blahV/math/blahV_math.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"
#include "blahV/vulkan/blahV_device.h"

typedef enum {
    BLV_CAMERA_PROJECTION_TYPE_PERSPECTIVE,
    BLV_CAMERA_PROJECTION_TYPE_ORTHOGRAPHIC,
} blvCameraProjectionType;

typedef enum {
    BLV_CAMERA_TYPE_SELF_MANAGED,
    BLV_CAMERA_TYPE_FREE_CAM,
} blvCameraType;

typedef struct {
    blvCameraType type;
    blvCameraProjectionType proj_type;
    float speed;
    float sensitivity;
    float fov;
    float near;
    float far;
    blvVec3 position;
    blvVec3 direction;
} blvCameraCreateInfo;

typedef struct {
    float speed;
    float sensitivity;
    float yaw;
    float pitch;
    float fov;
    float near;
    float far;
    blvCameraType type;
    blvCameraProjectionType proj_type;
    
    bool first_mouse;
    double last_mouse_pos_x;
    double last_mouse_pos_y;

    blvVec3 position;
    blvVec3 direction;
    blvVec3 up;
} blvCamera;

BLVAPI blvCamera blvCameraInit(blvCameraCreateInfo* create_info);

// Only for camera types != BLV_CAMERA_MANAGED
BLVAPI BLV_Result blvCameraUpdate(blvContext* context, blvCamera* camera, double* delta_time);
BLV_Result blvCameraUpdateBlvCams(blvContext* context, blvCamera* camera, double* delta_time);
void blvCameraUpdateFreeCam(blvContext* context, blvCamera* camera, double delta_time);
BLV_Result blvCameraSendMatrices(blvContext* context, blvCamera* camera);

#endif
