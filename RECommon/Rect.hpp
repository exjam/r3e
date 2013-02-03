#ifndef RECT_HPP
#define RECT_HPP

#include "Point.hpp"

class Rect {
public:
	Rect(){}
	Rect(int x1, int y1, int x2, int y2) : mMin(x1, y1), mMax(x2, y2) {}

	int Width(){
		return mMax.x - mMin.x;
	}

	int Height(){
		return mMax.y - mMin.y;
	}

	bool Contains(const Rect& rect){
		return (mMin <= rect.mMin) && (mMax >= rect.mMax);
	}

	bool Contains(const Point& point){
		return (mMin <= point) && (mMax >= point);
	}

public:
	Point mMin;
	Point mMax;
};

#endif