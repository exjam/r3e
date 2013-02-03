#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

#define GET_FLOAT_SIGN_BIT(x) ((*((int*)&x)) & (1 << 31))

#endif