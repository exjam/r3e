#ifndef RANDOM_H
#define RANDOM_H

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include <stdlib.h>

inline int Random(int min, int max){
	if(min == max) return min;
	return(rand() % (abs(max - min) + 1)) + min;
}

inline float Random(float min, float max){
	if(min == max) return min;
	float random = float(rand()) / float(RAND_MAX);
	return (random * (float)fabs(max - min)) + min;
}

inline Vector2 Random(Vector2 min, Vector2 max){
  return Vector2(Random(min.x, max.x), Random(min.y, max.y));
}

inline Vector3 Random(Vector3 min, Vector3 max){
  return Vector3(Random(min.x, max.x), Random(min.y, max.y), Random(min.z, max.z));
}

inline Vector4 Random(Vector4 min, Vector4 max){
  return Vector4(Random(min.x, max.x), Random(min.y, max.y), Random(min.z, max.z), Random(min.w, max.w));
}


#endif