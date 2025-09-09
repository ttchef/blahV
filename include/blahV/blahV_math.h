
#ifndef BLAHV_MATH_H
#define BLAHV_MATH_H

#include "HandmadeMath/HandmadeMath.h"

typedef HMM_Vec3 blvVec3;
typedef HMM_Mat4 blvMat4;


#define blvV3(x, y, z) (HMM_V3(x, y, z))
#define blvMat4Translate(vec3) (HMM_Translate(vec3))

#endif

