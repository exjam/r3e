#include "Global.h"

#include "TSI.hpp"

namespace ROSE {
	TSI::TSI(){}
	TSI::~TSI(){
		for(unsigned int i = 0; i < mImages.Size(); ++i)
			gUiRender->FreeImage(mImages[i]);

		mImages.Clear();
		mGraphics.Clear();
	}

	bool TSI::Load(const char* path){
		ScopedPointer<File> fh = gUiRender->OpenFile(path);
		if(!fh) return false;
		
		unsigned int images = fh->Read<short>();
		mImages.SetSize(images);
		for(unsigned int i = 0; i < images; ++i){
			mImages[i] = gUiRender->CreateImage(String("3DDATA\\CONTROL\\RES\\%1").arg(fh->ReadStringLength<short>()));
			fh->Skip(4);//colour
		}

		unsigned int totalElems = fh->Read<short>();
		mGraphics.SetSize(totalElems);
		int e = 0;
		for(unsigned int i = 0; i < images; ++i){
			unsigned int graphics = fh->Read<short>();
			for(unsigned int j = 0; j < graphics; ++j){
				Graphic* elem = &mGraphics[e];
				elem->mOwner = fh->Read<short>();
				elem->mRect = fh->Read<Rect>();
				fh->Skip(4);//colour
				fh->ReadData(elem->mName, 0x20);
				elem->mImage = mImages[elem->mOwner];
				++e;
			}
		}

		return true;
	}

	void TSI::AddToMap(StringMap<Graphic*>& map){
		for(unsigned int i = 0; i < mGraphics.Size(); ++i){
			Graphic* elem = &mGraphics[i];
			map.Add(elem->mName, elem);
		}
	}
};
