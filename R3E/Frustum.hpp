#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Matrix4.hpp"
#include "Plane.hpp"

class Frustum {
public:
	static Frustum CreateFromMatrix(const Matrix4& mat){
		Frustum ret;

		// Left
		ret.p[0].m[0] = mat.m[0][3] + mat.m[0][0];
		ret.p[0].m[1] = mat.m[1][3] + mat.m[1][0];
		ret.p[0].m[2] = mat.m[2][3] + mat.m[2][0];
		ret.p[0].m[3] = mat.m[3][3] + mat.m[3][0];

		// Right
		ret.p[1].m[0] = mat.m[0][3] - mat.m[0][0];
		ret.p[1].m[1] = mat.m[1][3] - mat.m[1][0];
		ret.p[1].m[2] = mat.m[2][3] - mat.m[2][0];
		ret.p[1].m[3] = mat.m[3][3] - mat.m[3][0];

		// Top
		ret.p[2].m[0] = mat.m[0][3] - mat.m[0][1];
		ret.p[2].m[1] = mat.m[1][3] - mat.m[1][1];
		ret.p[2].m[2] = mat.m[2][3] - mat.m[2][1];
		ret.p[2].m[3] = mat.m[3][3] - mat.m[3][1];

		// Bottom
		ret.p[3].m[0] = mat.m[0][3] + mat.m[0][1];
		ret.p[3].m[1] = mat.m[1][3] + mat.m[1][1];
		ret.p[3].m[2] = mat.m[2][3] + mat.m[2][1];
		ret.p[3].m[3] = mat.m[3][3] + mat.m[3][1];

		// Near
		ret.p[4].m[0] = mat.m[0][3] + mat.m[0][2];
		ret.p[4].m[1] = mat.m[1][3] + mat.m[1][2];
		ret.p[4].m[2] = mat.m[2][3] + mat.m[2][2];
		ret.p[4].m[3] = mat.m[3][3] + mat.m[3][2];

		// Far
		ret.p[5].m[0] = mat.m[0][3] - mat.m[0][2];
		ret.p[5].m[1] = mat.m[1][3] - mat.m[1][2];
		ret.p[5].m[2] = mat.m[2][3] - mat.m[2][2];
		ret.p[5].m[3] = mat.m[3][3] - mat.m[3][2];

		for(int i = 0; i < 6; ++i)
			ret.p[i].Normalise();

		return ret;
	};

	Plane p[6];
};

#endif
