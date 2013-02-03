#ifndef COLOUR_HPP
#define COLOUR_HPP

#pragma pack(push, 1)
struct Colour {
	Colour() : r(0xFF), g(0xFF), b(0xFF), a(0xFF) {}
	Colour(unsigned int c) : r(c & 0xFF), g((c >> 8) & 0xFF), b((c >> 16) & 0xFF), a((c >> 24) & 0xFF) {}
	Colour(unsigned char _r, unsigned char _g, unsigned char _b) : r(_r), g(_g), b(_b), a(0xFF) {}
	Colour(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) : r(_r), g(_g), b(_b), a(_a) {}

	Colour& operator =(const Colour& rhs){
		val = rhs.val;
		return *this;
	}

	Colour& operator =(const unsigned int& rhs){
		val = rhs;
		return *this;
	}

	bool operator ==(const unsigned int& rhs){
		return val == rhs;
	}

	bool operator !=(const unsigned int& rhs){
		return val != rhs;
	}

	union {
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};

		unsigned int val;
	};

	static Colour Red;
	static Colour Green;
	static Colour Blue;
	static Colour Black;
	static Colour White;
	static Colour Yellow;
	static Colour Gray;
	static Colour Violet;
	static Colour Orange;
};
#pragma pack(pop)

#endif