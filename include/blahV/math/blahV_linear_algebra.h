
#ifndef BLAHV_MATH_LINEAR_ALGEBRA_H
#define BLAHV_MATH_LINEAR_ALGEBRA_H

#include "HandmadeMath/HandmadeMath.h"

// Types
typedef HMM_Vec3 blvVec3;
typedef HMM_Vec4 blvVec4;
typedef HMM_Mat4 blvMat4;

// Type Getter functions ig
#define blvV3(x, y, z)      (HMM_V3(x, y, z))
#define blvV4(x, y, z, w)   (HMM_V4(x, y, z, w))

// Vector Math
#define blvV3MulF(vec, f) (HMM_MulV3F(vec, f))
#define blvV3Add(a, b) (HMM_AddV3(a, b))
#define blvV3Sub(a, b) (HMM_SubV3(a, b))
#define blvV3Mul(a, b) (HMM_MulV3(a, b))
#define blvV3Norm(vec) (HMM_NormV3(vec))
#define blvV3Cross(a, b) (HMM_Cross(a, b))

// Matrix Math 
#define blvMat4Mul(a, b) (HMM_MulM4(a, b))

// Matrix Transformation
#define blvMat4Translate(vec3) (HMM_Translate(vec3))
#define blvMat4Scale(vec3) (HMM_Scale(vec3))
#define blvMat4Perspective_RH_NO(fov, aspect_ratio, near, far) (HMM_Perspective_RH_NO(fov, aspect_ratio, near, far))
#define blvMat4Perspective_RH_ZO(fov, aspect_ratio, near, far) (HMM_Perspective_RH_ZO(fov, aspect_ratio, near, far))
#define blvMat4Perspective_LH_NO(fov, aspect_ratio, near, far) (HMM_Perspective_LH_NO(fov, aspect_ratio, near, far))
#define blvMat4Perspective_LH_ZO(fov, aspect_ratio, near, far) (HMM_Perspective_LH_ZO(fov, aspect_ratio, near, far))
#define blvMat4Orthographic_RH_NO(left, right, bottom, top, near, far) (HMM_Orthographic_RH_NO(left, right, bottom, top, near, far))
#define blvMat4Orthographic_RH_ZO(left, right, bottom, top, near, far) (HMM_Orthographic_RH_ZO(left, right, bottom, top, near, far))
#define blvMat4Orthographic_LH_NO(left, right, bottom, top, near, far) (HMM_Orthographic_LH_NO(left, right, bottom, top, near, far))
#define blvMat4Orthographic_LH_ZO(left, right, bottom, top, near, far) (HMM_Orthographic_LH_ZO(left, right, bottom, top, near, far))
#define blvMat4LookA_RH(eye, center, up) (HMM_LookAt_RH(eye, center, up))
#define blvMat4LookA_LH(eye, center, up) (HMM_LookAt_LH(eye, center, up))

#endif
