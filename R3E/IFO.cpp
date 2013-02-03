#include "Global.h"

#include "IFO.hpp"
#include "FileSystem.hpp"
#include "SceneManager.hpp"
#include "ScopedPointer.hpp"
#include "ObjectManager.hpp"

namespace ROSE {
	IFO::IFO(){}
	IFO::~IFO(){
		mObjects.Delete();
	}

	bool IFO::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;

		unsigned int blocks, type, offset;
		fh->Read(blocks);
		for(unsigned int i = 0; i < blocks; ++i){
			fh->Read(type);
			fh->Read(offset);
			long pos = fh->Position();
			fh->Seek(offset);
			ReadBlock(fh, type);
			fh->Seek(pos);
		}

		fh->Close();
		return true;
	}

	void IFO::LinkLightmaps(ROSE::LIT* deco, ROSE::LIT* cnst){
		for(unsigned int i = 0; i < deco->mObjectList.Size(); ++i){
			ROSE::LIT::Object* litobj = &deco->mObjectList[i];
			if(litobj->mIndex >= mObjects.Size()) continue;
			ObjectEntity* obj = mObjects[litobj->mIndex];
			obj->SetLitObject(litobj);
		}

		for(unsigned int i = 0; i < cnst->mObjectList.Size(); ++i){
			ROSE::LIT::Object* litobj = &cnst->mObjectList[i];
			if(litobj->mIndex >= mBuildings.Size()) continue;
			ObjectEntity* obj = mBuildings[litobj->mIndex];
			obj->SetLitObject(litobj);
		}
	}

	void IFO::AddObjectsToScene(SceneManager* scene){
		ObjectManager* objman = &ObjectManager::Instance();
		for(unsigned int i = 0; i < mObjects.Size(); ++i){
			ObjectEntity* obj = mObjects[i];
			obj->SetMesh(objman->LoadObject(obj->GetObjectType(), obj->GetObjectID()));
			scene->AddEntity(obj);
		}

		for(unsigned int i = 0; i < mBuildings.Size(); ++i){
			ObjectEntity* obj = mBuildings[i];
			obj->SetMesh(objman->LoadObject(obj->GetObjectType(), obj->GetObjectID()));
			scene->AddEntity(obj);
		}
	}

	void IFO::ReadBlock(File* fh, unsigned int type){
		if(type != DECORATIONS && type != BUILDINGS) return;
		unsigned int count;
		unsigned int objectID;
		Vector3 position, scale;
		Quaternion rotation;

		fh->Read(count);
		if(type == DECORATIONS) mObjects.Resize(count);
		if(type == BUILDINGS) mBuildings.Resize(count);
		for(unsigned int i = 0; i < count; ++i){
			fh->Skip(fh->Read<unsigned char>());
			fh->Skip(8);
			fh->Read(objectID);
			fh->Skip(8);
			fh->Read(rotation);
			fh->Read(position);
			fh->Read(scale);

			position /= 100.0f;
			position += Vector3(5200.0f, 5200.0f, 0.0f);

			ObjectEntity* obj = new ObjectEntity(type, objectID);
			obj->SetTransform(Matrix4::CreateRotation(rotation) * Matrix4::CreateScaling(scale) * Matrix4::CreateTranslation(position));

			if(type == DECORATIONS) mObjects.PushBack(obj);
			if(type == BUILDINGS) mBuildings.PushBack(obj);
		}
	}
};
