#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include "Frustum.hpp"
#include "Matrix4.hpp"
#include "Vector3.hpp"

class BoundingSphere {
public:
	BoundingSphere() : mRadius(0) {}
	~BoundingSphere(){}

	bool CheckOutsideFrustum(const Frustum& f) const {
		for(int i = 0; i < 6; ++i){
			if(f.p[i].DotProductCoord(mCenter) < -mRadius)
				return true;
		}

		return false;
	}

	Vector3 mCenter;
	float mRadius;
};

#endif
