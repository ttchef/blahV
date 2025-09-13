
#ifndef BLAHV_CAMERA_H
#define BLAHV_CAMERA_H

#include "blahV/math/blahV_math.h"
#include "blahV/core/blahV_log.h"
#include "blahV/core/blahV_context.h"

typedef enum {
    BLV_CAMERA_PROJECTION_TYPE_PERSPECTIVE,
    BLV_CAMERA_PROJECTION_TYPE_ORTHOGRAPHIC,
} blvCameraProjectionType;

typedef enum {
    BLV_CAMERA_SELF_MANAGED,
    BLV_CAMERA_FREE_MOVE,
} blvCameraType;

typedef struct {
    blvCameraType type;
    blvCameraProjectionType projection_type;
    float speed;
    float sensitivity;
    blvVec3 position;
    blvVec3 direction;
} blvCameraCreateInfo;

typedef struct {
    float speed;
    float sensitivity;
    float yaw;
    float pitch;

    blvVec3 position;
    blvVec3 direction;
    blvVec3 up;

    blvMat4 view_matrix;
    blvMat4 proj_matrix;
    blvMat4 view_proj_matrix;
} blvCamera;

BLVAPI blvCamera blvCameraInit(blvCameraCreateInfo* create_info);
void blvCameraDeinit();

#endif
