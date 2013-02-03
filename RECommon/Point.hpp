#ifndef POINT_HPP
#define POINT_HPP

class Point {
public:
	Point() : x(0), y(0) {}
	Point(int p) : x(short(p & 0xFFFF)), y(short((p >> 16) & 0xFFFF)) {}
	Point(int _x, int _y) : x(_x), y(_y) {}

	const Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}

	const Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}

	Point& operator+=(const Point& rhs){
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Point& operator-=(const Point& rhs){
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	bool operator!=(const Point& rhs) const {
		return (x != rhs.x)|| (y != rhs.y);
	}

	bool operator==(const Point& rhs) const {
		return (x == rhs.x) && (y == rhs.y);
	}

	bool operator<=(const Point& rhs) const {
		return (x <= rhs.x) && (y <= rhs.y);
	}

	bool operator>=(const Point& rhs) const {
		return (x >= rhs.x) && (y >= rhs.y);
	}

public:
	int x, y;
};

#endif
