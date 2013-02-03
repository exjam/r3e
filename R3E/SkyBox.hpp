#ifndef SKY_BOX_HPP
#define SKY_BOX_HPP

#include "Mesh1TexEntity.hpp"

class SkyBox : public Mesh1TexEntity {
public:
	SkyBox(){}
	~SkyBox(){}

	void SetDayTexture(const char* path){
		mDayTexure = path;
	}

	void SetNightTexture(const char* path){
		mNightTexure = path;
	}

private:
	String mDayTexure;
	String mNightTexure;
};

#endif