#ifndef MATRIX4_H
#define MATRIX4_H

#include "OpenGL.hpp"
#include "Math.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"

class Matrix4 {
public:
	Matrix4(){
		*this = IDENTITY;
	}

	Matrix4(float* s){
		float* tmp = (float*)this;
		for(int i = 0; i < 16; ++i)
			*tmp++ = *s++;
	}

	Matrix4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44){
		m[0][0] = m11;
		m[0][1] = m12;
		m[0][2] = m13;
		m[0][3] = m14;
		m[1][0] = m21;
		m[1][1] = m22;
		m[1][2] = m23;
		m[1][3] = m24;
		m[2][0] = m31;
		m[2][1] = m32;
		m[2][2] = m33;
		m[2][3] = m34;
		m[3][0] = m41;
		m[3][1] = m42;
		m[3][2] = m43;
		m[3][3] = m44;
	}

	Matrix4 operator*(const Matrix4& rhs) const {
		Matrix4 mat;
		for(int i = 0; i < 4; ++i){
			for(int j = 0; j < 4; ++j){
				mat.m[i][j] = m[i][0] * rhs.m[0][j] + m[i][1] * rhs.m[1][j] + m[i][2] * rhs.m[2][j] + m[i][3] * rhs.m[3][j];
			}
		}

		return mat;
	}

	void operator*=(const Matrix4& rhs) {
		(*this) = (*this) * rhs;
	}

	float Determinant() const {
		Vector4 minor, v1, v2, v3;
		v1.x = m[0][0]; v1.y = m[1][0]; v1.z = m[2][0]; v1.w = m[3][0];
		v2.x = m[0][1]; v2.y = m[1][1]; v2.z = m[2][1]; v2.w = m[3][1];
		v3.x = m[0][2]; v3.y = m[1][2]; v3.z = m[2][2]; v3.w = m[3][2];
		minor = Vector4::CreateCross(v1, v2, v3);
		return -(m[0][3] * minor.x + m[1][3] * minor.y + m[2][3] * minor.z + m[3][3] * minor.w);
	}

	Vector3 GetTranslate() const {
		return Vector3(m[3][0], m[3][1], m[3][2]);
	}

	float GetTranslateX() const {
		return m[3][0];
	}

	float GetTranslateY() const {
		return m[3][1];
	}

	float GetTranslateZ() const {
		return m[3][2];
	}

	Vector4 TransformVector4(const Vector4& v) const {
		Vector4 vec;
		vec.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w;
		vec.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w;
		vec.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w;
		vec.w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w;
		return vec;
	}

	Vector3 TransformCoord(const Vector3& v) const {
		Vector3 vec(0, 0, 0);

		float norm = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3];
		if(norm){
			vec.x = (m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0]) / norm;
			vec.y = (m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1]) / norm;
			vec.z = (m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]) / norm;
		}

		return vec;
	}

	Vector3 TransformNormal(const Vector3& v) const {
		Vector3 vec(0, 0, 0);

		vec.x = (m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z);
		vec.y = (m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z);
		vec.z = (m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);

		return vec;
	}

	void glLoad(){
		glLoadMatrixf(&m[0][0]);
	}

	void glMult(){
		glMultMatrixf(&m[0][0]);
	}

	static Matrix4 CreateInverse(const Matrix4& in){
		Matrix4 mat;
		Vector4 v, vec[3];

		float det = in.Determinant();
		for(int i = 0; i < 4; ++i){
			for(int j = 0; j < 4; ++j){
				if(j != i){
					int a = j;
					if(j > i) a = a - 1;
					vec[a].x = in.m[j][0];
					vec[a].y = in.m[j][1];
					vec[a].z = in.m[j][2];
					vec[a].w = in.m[j][3];
				}
			}

			v = Vector4::CreateCross(vec[0], vec[1], vec[2]);
			mat.m[0][i] = pow(-1.0f, i) * v.x / det;
			mat.m[1][i] = pow(-1.0f, i) * v.y / det;
			mat.m[2][i] = pow(-1.0f, i) * v.z / det;
			mat.m[3][i] = pow(-1.0f, i) * v.w / det;
		}

		return mat;
	}

	static Matrix4 CreatePerspectiveRH(float w, float h, float zn, float zf){
		Matrix4 mat = IDENTITY;
		mat.m[0][0] = 2.0f * zn / w;
		mat.m[1][1] = 2.0f * zn / h;
		mat.m[2][2] = zf / (zn - zf);
		mat.m[3][2] = (zn * zf) / (zn - zf);
		mat.m[2][3] = -1.0f;
		mat.m[3][3] = 0.0f;
		return mat;
	}

	static Matrix4 CreatePerspectiveLH(float w, float h, float zn, float zf){
		Matrix4 mat = IDENTITY;
		mat.m[0][0] = 2.0f * zn / w;
		mat.m[1][1] = 2.0f * zn / h;
		mat.m[2][2] = zf / (zf - zn);
		mat.m[3][2] = (zn * zf) / (zn - zf);
		mat.m[2][3] = 1.0f;
		mat.m[3][3] = 0.0f;
		return mat;
	}

	static Matrix4 CreatePerspectiveFovRH(float fovy, float aspect, float zn, float zf){
		Matrix4 mat = IDENTITY;
		mat.m[0][0] = 1.0f / (aspect * tan(fovy / 2.0f));
		mat.m[1][1] = 1.0f / tan(fovy / 2.0f);
		mat.m[2][2] = zf / (zn - zf);
		mat.m[2][3] = -1.0f;
		mat.m[3][2] = (zf * zn) / (zn - zf);
		mat.m[3][3] = 0.0f;
		return mat;
	}

	static Matrix4 CreateRotation(const Quaternion& rot){
		Matrix4 mat = IDENTITY;
		mat.m[0][0] = 1.0f - 2.0f * (rot.y * rot.y + rot.z * rot.z);
		mat.m[0][1] = 2.0f * (rot.x *rot.y + rot.z * rot.s);
		mat.m[0][2] = 2.0f * (rot.x * rot.z - rot.y * rot.s);
		mat.m[1][0] = 2.0f * (rot.x * rot.y - rot.z * rot.s);
		mat.m[1][1] = 1.0f - 2.0f * (rot.x * rot.x + rot.z * rot.z);
		mat.m[1][2] = 2.0f * (rot.y *rot.z + rot.x *rot.s);
		mat.m[2][0] = 2.0f * (rot.x * rot.z + rot.y * rot.s);
		mat.m[2][1] = 2.0f * (rot.y *rot.z - rot.x *rot.s);
		mat.m[2][2] = 1.0f - 2.0f * (rot.x * rot.x + rot.y * rot.y);
		return mat;
	}

	static Matrix4 CreateTranslation(Vector3 pos){
		Matrix4 mat = IDENTITY;
		mat.m[3][0] = pos.x;
		mat.m[3][1] = pos.y;
		mat.m[3][2] = pos.z;
		return mat;
	}

	static Matrix4 CreateTranslation2d(Vector3 pos){
		Matrix4 mat = IDENTITY;
		mat.m[2][0] = pos.x;
		mat.m[2][1] = pos.y;
		mat.m[2][2] = pos.z;
		return mat;
	}

	static Matrix4 CreateRotation(float yaw, float pitch, float roll){
		Matrix4 mat = IDENTITY;
		mat *= CreateRotationZ(roll);
		mat *= CreateRotationX(pitch);
		mat *= CreateRotationY(yaw);
		return mat;
	}

	static Matrix4 CreateScaling(Vector3 scale){
		Matrix4 mat = IDENTITY;
		mat.m[0][0] = scale.x;
		mat.m[1][1] = scale.y;
		mat.m[2][2] = scale.z;
		return mat;
	}

	static Matrix4 CreateRotationX(float angle){
		Matrix4 mat = IDENTITY;
		mat.m[1][1] = cos(angle);
		mat.m[2][2] = cos(angle);
		mat.m[1][2] = sin(angle);
		mat.m[2][1] = -sin(angle);
		return mat;
	}

	static Matrix4 CreateRotationY(float angle){
		Matrix4 mat = IDENTITY;
		mat.m[0][0] = cos(angle);
		mat.m[2][2] = cos(angle);
		mat.m[0][2] = -sin(angle);
		mat.m[2][0] = sin(angle);
		return mat;
	}

	static Matrix4 CreateRotationZ(float angle){
		Matrix4 mat = IDENTITY;
		mat.m[0][0] = cos(angle);
		mat.m[1][1] = cos(angle);
		mat.m[0][1] = sin(angle);
		mat.m[1][0] = -sin(angle);
		return mat;
	}

	static Matrix4 CreateLookAtRH(const Vector3& eye, const Vector3& target, const Vector3& up){
		Matrix4 mat;
		Vector3 right, rightn, upa, upn, vec, vec2;
		vec2 = target - eye;
		vec = Vector3::CreateNormalised(vec2);
		right = Vector3::CreateCross(up, vec);
		upa = Vector3::CreateCross(vec, right);
		rightn = Vector3::CreateNormalised(right);
		upn = Vector3::CreateNormalised(upa);
		mat.m[0][0] = -rightn.x;
		mat.m[1][0] = -rightn.y;
		mat.m[2][0] = -rightn.z;
		mat.m[3][0] = Vector3::CreateDotProduct(rightn, eye);
		mat.m[0][1] = upn.x;
		mat.m[1][1] = upn.y;
		mat.m[2][1] = upn.z;
		mat.m[3][1] = -Vector3::CreateDotProduct(upn, eye);
		mat.m[0][2] = -vec.x;
		mat.m[1][2] = -vec.y;
		mat.m[2][2] = -vec.z;
		mat.m[3][2] = Vector3::CreateDotProduct(vec, eye);
		mat.m[0][3] = 0.0f;
		mat.m[1][3] = 0.0f;
		mat.m[2][3] = 0.0f;
		mat.m[3][3] = 1.0f;
		return mat;
	}

	static Matrix4 CreateOrthoLH(float w, float h, float zn, float zf){
		Matrix4 mat = IDENTITY;
		mat.m[0][0] = 2.0f / w;
		mat.m[1][1] = 2.0f / h;
		mat.m[2][2] = 1.0f / (zf - zn);
		mat.m[3][2] = zn / (zn - zf);
		return mat;
	}

	static const Matrix4 ZERO;
	static const Matrix4 IDENTITY;

	float m[4][4];
};

#endif
