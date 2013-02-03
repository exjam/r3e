#ifndef PLANE_H
#define PLANE_H

class Plane {
public:
	Plane(){
		for(int i = 0; i < 4; ++i)
			m[i] = 0.0f;
	}

	Plane(float d){
		for(int i = 0; i < 4; ++i)
			m[i] = d;
	}

	Plane(float a, float b, float c, float d){
		m[0] = a;
		m[1] = b;
		m[2] = c;
		m[3] = d;
	}

	~Plane(){}

	float Normalise(){
		float norm = sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
		if(norm) {
			for(int i = 0; i < 4; ++i)
				m[i] /= norm;
		}else{
			for(int i = 0; i < 4; ++i)
				m[i] = 0.0f;
		}

		return norm;
	};

	float DotProductCoord(const Vector3& v) const {
		return m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3];
	};

	float m[4];
};

#endif
