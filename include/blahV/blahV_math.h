
#ifndef BLAHV_MATH_H
#define BLAHV_MATH_H

#include "HandmadeMath/HandmadeMath.h"

// Types
typedef HMM_Vec3 blvVec3;
typedef HMM_Mat4 blvMat4;

// Type Getter functions ig
#define blvV3(x, y, z) (HMM_V3(x, y, z))

// Matrix Math 
#define blvMat4Mul(a, b) (HMM_MulM4(a, b))

// Matrix Transformation
#define blvMat4Translate(vec3) (HMM_Translate(vec3))
#define blvMat4Scale(vec3) (HMM_Scale(vec3))

#endif

