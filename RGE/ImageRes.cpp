#include "Global.h"

#include "ImageRes.hpp"

ImageRes ImageRes::mSelf;

ImageRes::ImageRes(){
	memset(mTSI, 0, sizeof(ROSE::TSI*) * IMAGE_RES_MAX);
}

ImageRes::~ImageRes(){
	mGraphicID.Clear();

	for(unsigned int i = 0; i < IMAGE_RES_MAX; ++i){
		SAFE_DELETE(mTSI[i]);
	}
}

void ImageRes::Load(){
	static const char* paths[] = {
		"3DDATA\\CONTROL\\RES\\UI.TSI",
		"3DDATA\\CONTROL\\RES\\ITEM1.TSI",
		NULL,
		"3DDATA\\CONTROL\\RES\\EXUI.TSI",
		"3DDATA\\CONTROL\\RES\\SKILLICON.TSI",
		"3DDATA\\CONTROL\\RES\\STATEICON.TSI",
		"3DDATA\\CONTROL\\RES\\SOKETJAM.TSI",
		"3DDATA\\CONTROL\\RES\\CLANBACK.TSI",
		"3DDATA\\CONTROL\\RES\\CLANCENTER.TSI",
		"3DDATA\\CONTROL\\RES\\TARGETMARK.TSI",
		NULL
	};

	for(unsigned int i = IMAGE_RES_UI; i < IMAGE_RES_MAX; ++i){
		if(!paths[i]) continue;
		mTSI[i] = new ROSE::TSI();

		if(!mTSI[i]->Load(paths[i]))
			LOG("Error loading '%s'", paths[i]);
		else
			mTSI[i]->AddToMap(mGraphicID);
	}
}

ROSE::TSI::Graphic* ImageRes::GetGraphic(const char* gid){
	if(!gid) return NULL;

	StringMap<ROSE::TSI::Graphic*>::Iterator itr = mGraphicID.Find(gid);
	if(itr == mGraphicID.End()) return NULL;
	return *itr;
}
