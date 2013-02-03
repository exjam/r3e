#include "Global.h"

#include "LIT.hpp"
#include "FileSystem.hpp"
#include "ScopedPointer.hpp"

namespace ROSE {
	LIT::LIT(){}
	LIT::~LIT(){}
	
	bool LIT::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;
		unsigned int objects, parts;

		String directory = path;
		directory.Null((int)(strrchr(path, '\\') - path));

		fh->Read(objects);
		mObjectList.SetSize(objects);
		for(unsigned int i = 0; i < objects; ++i){
			Object* obj = &mObjectList[i];

			fh->Read(parts);
			fh->Read(obj->mIndex);

			--obj->mIndex;

			obj->mParts.SetSize(parts);
			for(unsigned int j = 0; j < parts; ++j){
				Object::Part* part = &obj->mParts[j];
				fh->Skip(fh->Read<unsigned char>());
				fh->Read(part->mIndex);

				String texture = String("%1\\%2").arg(directory.Str()).arg(fh->ReadStringLength<unsigned char>());
				part->mTexture = TEX_MGR().Load(texture);

				int mapIndex, objPerRow;

				fh->Skip(8);
				fh->Read(objPerRow);
				fh->Read(mapIndex);

				int x = mapIndex % objPerRow;
				int y = mapIndex / objPerRow;
				float scale = 1.0f / float(objPerRow);

				part->xPos = float(x) * scale;
				part->yPos = float(y) * scale;
				part->scale = scale;

				part->mTransform = Matrix4::CreateScaling(Vector3(scale, scale, 1.0f));
				part->mTransform *= Matrix4::CreateTranslation(Vector3(float(x) * scale, float(y) * scale, 1.0f));
			}
		}

		fh->Close();
		return true;
	}
};