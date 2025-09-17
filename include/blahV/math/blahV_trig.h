
#ifndef BLAHV_MATH_TRIG_H
#define BLAHV_MATH_TRIG_H

#include <math.h>

// Trigometrix functions 
#define blvMathSin(x) (sin(x))
#define blvMathCos(x) (cos(x))
#define blvMathTan(x) (tan(x))

// Functions
static inline double blvMathDegToRad(double deg) {
    return deg * M_PI / 180.0;
}

static inline double blvMathRadToDeg(double rad) {
    return rad * 180.0 / M_PI;
}

#endif
