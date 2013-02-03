#ifndef IMAGE_RES_HPP
#define IMAGE_RES_HPP

enum {	
	IMAGE_RES_UI,
	IMAGE_RES_ITEM,
	IMAGE_RES_NPCFACE,
	IMAGE_RES_EXUI,
	IMAGE_RES_SKILL_ICON,
	IMAGE_RES_STATE_ICON,
	IMAGE_RES_SOCKETJAM_ICON,
	IMAGE_RES_CLANBACK,
	IMAGE_RES_CLANCENTER,
	IMAGE_RES_TARGETMARK,
	IMAGE_RES_MAX,
};

#include "StringMap.hpp"
#include "TSI.hpp"
#include "Log.hpp"

class ImageRes {
private:
	static ImageRes mSelf;

public:
	ImageRes();
	~ImageRes();

	static ImageRes& Instance(){
		return mSelf;
	}

	void Load();
	ROSE::TSI::Graphic* GetGraphic(const char* gid);

private:
	ROSE::TSI* mTSI[IMAGE_RES_MAX];
	StringMap<ROSE::TSI::Graphic*> mGraphicID;
};

#endif