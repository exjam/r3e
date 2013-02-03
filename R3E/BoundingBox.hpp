#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "OpenGL.hpp"
#include "Math.hpp"
#include "Frustum.hpp"
#include "Matrix4.hpp"
#include "Vector3.hpp"

class BoundingBox {
public:
	BoundingBox(){Reset();}
	BoundingBox(float p) : mMin(p), mMax(p) {}
	BoundingBox(Vector3 p) : mMin(p), mMax(p) {}
	BoundingBox(Vector3 mn, Vector3 mx) : mMin(mn), mMax(mx) {}

	~BoundingBox(){}

	void Reset(){
		mMin = Vector3(9999.0f);
		mMax = Vector3(-9999.0f);
	}

	BoundingBox operator/(float rhs) const {
		return BoundingBox(mMin / rhs, mMax / rhs);
	}

	void operator/=(float rhs){
		(*this) = (*this) / rhs;
	}

	void AddBox(const BoundingBox& box){
		AddPoint(box.mMin);
		AddPoint(box.mMax);
	}

	void AddTransformedBox(const BoundingBox& b, const Matrix4& mat){
		AddPoint(mat.TransformCoord(Vector3(b.mMin.x, b.mMin.y, b.mMin.z)));
		AddPoint(mat.TransformCoord(Vector3(b.mMin.x, b.mMax.y, b.mMin.z)));
		AddPoint(mat.TransformCoord(Vector3(b.mMax.x, b.mMin.y, b.mMin.z)));
		AddPoint(mat.TransformCoord(Vector3(b.mMax.x, b.mMax.y, b.mMin.z)));

		AddPoint(mat.TransformCoord(Vector3(b.mMin.x, b.mMin.y, b.mMax.z)));
		AddPoint(mat.TransformCoord(Vector3(b.mMin.x, b.mMax.y, b.mMax.z)));
		AddPoint(mat.TransformCoord(Vector3(b.mMax.x, b.mMin.y, b.mMax.z)));
		AddPoint(mat.TransformCoord(Vector3(b.mMax.x, b.mMax.y, b.mMax.z)));
	}

	void AddPoint(const Vector3& vec){
		if(mMin.x > vec.x) mMin.x = vec.x;
		if(mMin.y > vec.y) mMin.y = vec.y;
		if(mMin.z > vec.z) mMin.z = vec.z;

		if(mMax.x < vec.x) mMax.x = vec.x;
		if(mMax.y < vec.y) mMax.y = vec.y;
		if(mMax.z < vec.z) mMax.z = vec.z;
	}

	bool Contains(const BoundingBox& other) const {
		if(mMax.x < other.mMax.x) return false;
		if(mMax.y < other.mMax.y) return false;
		if(mMax.z < other.mMax.z) return false;

		if(mMin.x > other.mMin.x) return false;
		if(mMin.y > other.mMin.y) return false;
		if(mMin.z > other.mMin.z) return false;

		return true;
	}

	bool Contains2D(const Vector3& other) const {
		if(mMax.x < other.x) return false;
		if(mMax.y < other.y) return false;
		if(mMin.x > other.x) return false;
		if(mMin.y > other.y) return false;

		return true;
	}

	int CheckIntersectFrustum(const Frustum& f) const {
		//2 = Completely In
		//1 = Intersect
		//0 = Completely Out

		int ret = 2;
		for(int i = 0; i < 6; ++i){
			Vector3 p(mMin);
			Vector3 n(mMax);

			if(!GET_FLOAT_SIGN_BIT(f.p[i].m[0])){
				p.x = mMax.x;
				n.x = mMin.x;
			}

			if(!GET_FLOAT_SIGN_BIT(f.p[i].m[1])){
				p.y = mMax.y;
				n.y = mMin.y;
			}

			if(!GET_FLOAT_SIGN_BIT(f.p[i].m[2])){
				p.z = mMax.z;
				n.z = mMin.z;
			}

			float dotProduct = f.p[i].DotProductCoord(p);
			if(GET_FLOAT_SIGN_BIT(dotProduct))
				return 0;

			dotProduct = f.p[i].DotProductCoord(n);
			if(GET_FLOAT_SIGN_BIT(dotProduct))
				ret = 1;
		}

		return ret;
	}

	void Render(){
		glBegin(GL_LINE_LOOP);
		glVertex3f(mMin.x, mMin.y, mMax.z);
		glVertex3f(mMax.x, mMin.y, mMax.z);
		glVertex3f(mMax.x, mMax.y, mMax.z);
		glVertex3f(mMin.x, mMax.y, mMax.z);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex3f(mMin.x, mMin.y, mMin.z);
		glVertex3f(mMin.x, mMax.y, mMin.z);
		glVertex3f(mMax.x, mMax.y, mMin.z);
		glVertex3f(mMax.x, mMin.y, mMin.z);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex3f(mMin.x, mMax.y, mMin.z);
		glVertex3f(mMin.x, mMax.y, mMax.z);
		glVertex3f(mMax.x, mMax.y, mMax.z);
		glVertex3f(mMax.x, mMax.y, mMin.z);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex3f(mMin.x, mMin.y, mMin.z);
		glVertex3f(mMax.x, mMin.y, mMin.z);
		glVertex3f(mMax.x, mMin.y, mMax.z);
		glVertex3f(mMin.x, mMin.y, mMax.z);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex3f(mMax.x, mMin.y, mMin.z);
		glVertex3f(mMax.x, mMax.y, mMin.z);
		glVertex3f(mMax.x, mMax.y, mMax.z);
		glVertex3f(mMax.x, mMin.y, mMax.z);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex3f(mMin.x, mMin.y, mMin.z);
		glVertex3f(mMin.x, mMin.y, mMax.z);
		glVertex3f(mMin.x, mMax.y, mMax.z);
		glVertex3f(mMin.x, mMax.y, mMin.z);
		glEnd();
	}

	Vector3 mMin, mMax;
};

#endif
