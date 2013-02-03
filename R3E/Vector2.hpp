#ifndef VECTOR2_H
#define VECTOR2_H

#include "Math.hpp"

class Vector2 {
public:
	Vector2() : x(0), y(0) {}
	Vector2(float s) : x(s), y(s) {}
	Vector2(float x_, float y_) : x(x_), y( y_) {}

	const Vector2 operator -(void) const {
		return Vector2(-x, -y);
	}

	Vector2 operator+(const Vector2& rhs) const {
		return Vector2(x + rhs.x, y + rhs.y);
	}

	Vector2 operator-(const Vector2& rhs) const {
		return Vector2(x - rhs.x, y - rhs.y);
	}

	Vector2 operator*(const Vector2& rhs) const {
		return Vector2(x * rhs.x, y * rhs.y);
	}

	Vector2 operator/(const Vector2& rhs) const {
		return Vector2(x / rhs.x, y / rhs.y);
	}

	void operator+=(const Vector2& rhs){
		(*this) = (*this) + rhs;
	}

	void operator-=(const Vector2& rhs){
		(*this) = (*this) - rhs;
	}

	void operator*=(const Vector2& rhs){
		(*this) = (*this) * rhs;
	}

	void operator/=(const Vector2& rhs){
		(*this) = (*this) / rhs;
	}

	float Length() const {
		return sqrt(x * x + y * y);
	}

	float LengthSq() const {
		return x * x + y * y;
	}

	bool IsZero() const {
		return (x + y) < 0.001f;
	}

	float Normalise(){
		float norm  = Length();

		if(!norm){
			x = 0.0f;
			y = 0.0f;
		}else{
			x = x / norm;
			y = y / norm;
		}

		return norm;
	}


	float x, y;
};

#endif
