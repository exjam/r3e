#ifndef VECTOR4_H
#define VECTOR4_H

#include "Math.hpp"

class Vector4 {
public:
	Vector4() : x(0), y(0), z(0), w(0) {}
	Vector4(float s) : x(s), y(s), z(s), w(s) {}
	Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

	const Vector4 operator -(void) const {
		return Vector4(-x, -y, -z, -w);
	}

	Vector4 operator+(const Vector4& rhs) const {
		return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	Vector4 operator-(const Vector4& rhs) const {
		return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	Vector4 operator*(const Vector4& rhs) const {
		return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
	}

	Vector4 operator/(const Vector4& rhs) const {
		return Vector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
	}

	void operator+=(const Vector4& rhs){
		(*this) = (*this) + rhs;
	}

	void operator-=(const Vector4& rhs){
		(*this) = (*this) - rhs;
	}

	void operator*=(const Vector4& rhs){
		(*this) = (*this) * rhs;
	}

	void operator/=(const Vector4& rhs){
		(*this) = (*this) / rhs;
	}

	float Length() const {
		return sqrt(x * x + y * y + z * z + w * w);
	}

	float LengthSq() const {
		return x * x + y * y + z * z + w * w;
	}

	bool IsZero() const {
		return (x + y + z + w) < 0.001f;
	}

	float Normalise(){
		float norm  = Length();

		if(!norm){
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}else{
			x = x / norm;
			y = y / norm;
			z = z / norm;
			w = w / norm;
		}

		return norm;
	}

	static Vector4 CreateCross(const Vector4& v1, const Vector4& v2, const Vector4& v3){
		Vector4 v;
		v.x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
		v.y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
		v.z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
		v.w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
		return v;
	};

	float x, y, z, w;
};

#endif
