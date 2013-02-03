#include "Global.h"

#include "HIM.hpp"
#include "FileSystem.hpp"
#include "ScopedPointer.hpp"

namespace ROSE {
	HIM::HIM() : mWidth(0), mHeight(0) {}
	HIM::~HIM(){}

	void HIM::Unload(){
	}

	bool HIM::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;

		fh->Read(mWidth);
		fh->Read(mHeight);
		fh->Read(mGridCount);
		fh->Read(mGridSize);
		fh->ReadData(mHeights, 65*65*4);

		for(int x = 0; x < 65; ++x){
			for(int y = 0; y < 65; ++y) mHeights[x][y] /= 100.0f;
		}

		fh->Close();

		return true;
	}

	int HIM::Width() const {
		return mWidth;
	}

	int HIM::Height() const {
		return mHeight;
	}

	float HIM::GetHeight(int x, int y) const {
		return mHeights[y][x];
	}

	float** HIM::Heights() const {
		return (float**)mHeights;
	}
};