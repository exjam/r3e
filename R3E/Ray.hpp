#ifndef RAY_HPP
#define RAY_HPP

#include "Vector3.hpp"

#include "SlopeIntMul.hpp"

class Ray {
public:
	Ray(){}
	Ray(Vector3 point, Vector3 dir) : mPoint(point), mDirection(dir) {}

	Vector3 GetPosition(float t) const {
		return mPoint + mDirection*t;
	}

	bool IntersectsBox(const BoundingBox& bb) const {
		SlopeRay ray;
		ray.make_ray(mPoint.x, mPoint.y, mPoint.z, mDirection.x, mDirection.y, mDirection.z);
		return slopeint_mul(&ray, &bb);
	}

	bool IntersectsBox(const BoundingBox& bb, Vector3& pos) const {
		SlopeRay ray;
		ray.make_ray(mPoint.x, mPoint.y, mPoint.z, mDirection.x, mDirection.y, mDirection.z);
		float t;
		if(!slopeint_mul(&ray, &bb, &t)) return false;
		pos = GetPosition(t);
		return true;
	}

	bool IntersectsBox(const BoundingBox& bb, float& t) const {
		SlopeRay ray;
		ray.make_ray(mPoint.x, mPoint.y, mPoint.z, mDirection.x, mDirection.y, mDirection.z);
		return slopeint_mul(&ray, &bb, &t);
	}

	bool IntersectTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2) const {
		Vector3 edge1, edge2, pvec, tvec, qvec;
		float det, u, v;

		edge1 = v1 - v0;
		edge2 = v2 - v0;

		pvec = Vector3::CreateCross(mDirection, edge2);
		det = Vector3::CreateDotProduct(edge1, pvec);

		if(det < 0.0001f)
			return false;

		tvec = mPoint - v0;

		u = Vector3::CreateDotProduct(tvec, pvec);
		if(u < 0.0 || u > det)
			return false;

		qvec = Vector3::CreateCross(tvec, edge1);

		v = Vector3::CreateDotProduct(mDirection, qvec);
		if(v < 0.0 || u + v > det)
			return false;

		return true;
	}

	bool IntersectTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, float& t) const {
		Vector3 edge1, edge2, pvec, tvec, qvec;
		float det, u, v;

		edge1 = v1 - v0;
		edge2 = v2 - v0;

		pvec = Vector3::CreateCross(mDirection, edge2);
		det = Vector3::CreateDotProduct(edge1, pvec);

		if(det < 0.0001f)
			return false;

		tvec = mPoint - v0;

		u = Vector3::CreateDotProduct(tvec, pvec);
		if(u < 0.0 || u > det)
			return false;

		qvec = Vector3::CreateCross(tvec, edge1);

		v = Vector3::CreateDotProduct(mDirection, qvec);
		if(v < 0.0 || u + v > det)
			return false;

		t = Vector3::CreateDotProduct(edge2, qvec);
		float inv_det = 1.0f / det;
		t *= inv_det;

		return true;
	}

public:
	Vector3 mPoint;
	Vector3 mDirection;
};

#endif
