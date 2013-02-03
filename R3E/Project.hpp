#ifndef UNPROJECT_HPP
#define UNPROJECT_HPP

#include "Ray.hpp"
#include "Point.hpp"
#include "Matrix4.hpp"

static int Project(const Vector3& obj, const Matrix4& _modelview, const Matrix4& _projection, const Point& view, Vector3& out){
	float fTempo[8];
	const float* modelview = (const float*)_modelview.m;
	const float* projection = (const float*)_projection.m;

	//Modelview transform
	fTempo[0] = modelview[0]*obj.x + modelview[4]*obj.y + modelview[8]*obj.z + modelview[12];  //w is always 1
	fTempo[1] = modelview[1]*obj.x + modelview[5]*obj.y + modelview[9]*obj.z + modelview[13];
	fTempo[2] = modelview[2]*obj.x + modelview[6]*obj.y + modelview[10]*obj.z + modelview[14];
	fTempo[3] = modelview[3]*obj.x + modelview[7]*obj.y + modelview[11]*obj.z + modelview[15];

	//Projection transform, the final row of projection matrix is always [0 0 -1 0]
	//so we optimize for that.
	fTempo[4] = projection[0]*fTempo[0] + projection[4]*fTempo[1] + projection[8]*fTempo[2] + projection[12]*fTempo[3];
	fTempo[5] = projection[1]*fTempo[0] + projection[5]*fTempo[1] + projection[9]*fTempo[2] + projection[13]*fTempo[3];
	fTempo[6] = projection[2]*fTempo[0] + projection[6]*fTempo[1] + projection[10]*fTempo[2] + projection[14]*fTempo[3];
	fTempo[7] = -fTempo[2];

	//The result normalizes between -1 and 1
	if(fTempo[7] == 0.0)
		return 0;

	fTempo[7] = 1.0f / fTempo[7];

	//Perspective division
	fTempo[4] *= fTempo[7];
	fTempo[5] *= fTempo[7];
	fTempo[6] *= fTempo[7];

	//Window coordinates
	out.x = (fTempo[4]*0.5f + 0.5f) * float(view.x);
	out.y = (1.0f - (fTempo[5]*0.5f + 0.5f)) * float(view.y);
	out.z = (1.0f + fTempo[6]) * 0.5f;

	return 1;
}

static int Unproject(const Vector3& mouse, const Matrix4& modelview, const Matrix4& projection, const Vector2& view, Vector3& out){
	Matrix4 invmp = Matrix4::CreateInverse(modelview * projection);
	
	Vector4 in;
	in.x = mouse.x / view.x * 2.0f - 1.0f;
	in.y = mouse.y / view.y * 2.0f - 1.0f;
	in.z = 2.0f * mouse.z - 1.0f;
	in.w = 1.0f;

	Vector4 trans = invmp.TransformVector4(in);
	if(trans.w == 0.0f)
		return 0;

	trans.w  = 1.0f / trans.w;
	out.x = trans.x * trans.w;
	out.y = trans.y * trans.w;
	out.z = trans.z * trans.w;

	return 1;
}

static Ray UnprojectRay(Vector3& mouse, const Matrix4& modelview, const Matrix4& projection, const Vector2& view){
	Vector3 npos, fpos;
	mouse.z = 0.0f;
	Unproject(mouse, modelview, projection, view, npos);

	mouse.z = 1.0f;
	Unproject(mouse, modelview, projection, view, fpos);

	Vector3 direction = (fpos - npos);
	direction.Normalise();

	Ray ray(npos, direction);
	return ray;
}

#endif
