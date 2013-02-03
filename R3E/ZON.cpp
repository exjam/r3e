#include "Global.h"

#include "ZON.hpp"
#include "FileSystem.hpp"
#include "ScopedPointer.hpp"

namespace ROSE {
	ZON::ZON(){}
	ZON::~ZON(){
		mTextureList.Clear();
		mTileList.Clear();
	}
	
	bool ZON::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;

		unsigned int blocks, type, offset;
		fh->Read(blocks);
		for(unsigned int i = 0; i < blocks; ++i){
			fh->Read(type);
			fh->Read(offset);
			long pos = fh->Position();
			switch(type){
				case TEXTURE_LIST:
					fh->Seek(offset);
					ReadTextureList(fh);
					break;
				case TILE_LIST:
					fh->Seek(offset);
					ReadTileList(fh);
					break;
			}

			fh->Seek(pos);
		}

		fh->Close();
		return true;
	}

	SmartPointer<Texture> ZON::GetTexture(int id) const {
		return mTextureList[id];
	}

	ZON::Tile* ZON::GetTile(int id) const {
		return &mTileList[id];
	}

	void ZON::ReadTextureList(File* fh){
		unsigned int count;
		fh->Read(count);
		mTextureList.SetSize(count);
		for(unsigned int i = 0; i < count; ++i)
			mTextureList[i] = TEX_MGR().Load(fh->ReadStringLength<unsigned char>());
	}

	void ZON::ReadTileList(File* fh){
		unsigned int count;
		fh->Read(count);
		mTileList.SetSize(count);
		for(unsigned int i = 0; i < count; ++i){
			Tile* tile = &mTileList[i];
			fh->Read(tile->mLayer1);
			fh->Read(tile->mLayer2);
			fh->Read(tile->mOffset1);
			fh->Read(tile->mOffset2);
			fh->Read(tile->mBlending);
			fh->Read(tile->mRotation);
			fh->Read(tile->mType);
		}
	}
};
