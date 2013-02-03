#include "Global.h"

#include "TIL.hpp"
#include "FileSystem.hpp"
#include "ScopedPointer.hpp"

namespace ROSE {
	TIL::TIL() : mWidth(0), mHeight(0) {}
	TIL::~TIL(){}

	bool TIL::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;

		fh->Read(mWidth);
		fh->Read(mHeight);
		for(int x = 0; x < mWidth; ++x){
			for(int y = 0; y < mHeight; ++y){
				fh->Read(mTileMap[x][y].mBrushNumber);
				fh->Read(mTileMap[x][y].mTileIndex);
				fh->Read(mTileMap[x][y].mTileSet);
				fh->Read(mTileMap[x][y].mTileNumber);
			}
		}

		fh->Close();

		return true;
	}

	void TIL::Unload(){
	}

	int TIL::Width() const {
		return mWidth;
	}

	int TIL::Height() const {
		return mHeight;
	}

	int TIL::GetTile(int x, int y) const {
		return mTileMap[y][x].mTileNumber;
	}
};