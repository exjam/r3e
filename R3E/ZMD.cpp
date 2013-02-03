#include "Global.h"

#include "ZMD.hpp"
#include "Log.hpp"
#include "FileSystem.hpp"
#include "ScopedPointer.hpp"

namespace ROSE {
	ZMD::ZMD(){}
	ZMD::~ZMD(){}

	void ZMD::Unload(){
		mBoneList.Clear();
	}

	bool ZMD::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;

		char VersionHeader[7];
		fh->ReadData(VersionHeader, 7);
		if(strncmp(VersionHeader, "ZMD0003", 7) == 0){
			mVersion = 3;
		}else if(strncmp(VersionHeader, "ZMD0002", 7) == 0){
			mVersion = 2;
		}else{
			LOG("Error: '%s' invalid version '%s'", path, VersionHeader);
			fh->Close();
			return false;
		}

		unsigned int boneCount;
		fh->Read(boneCount);
		mBoneList.SetSize(boneCount);
		for(unsigned int i = 0; i < boneCount; ++i){
			Bone* bone = &mBoneList[i];
			bone->mDummy = false;
			fh->Read(bone->mParentID);
			bone->mName = fh->ReadTerminatedString();
			fh->Read(bone->mTranslate.x);
			fh->Read(bone->mTranslate.y);
			fh->Read(bone->mTranslate.z);

			fh->Read(bone->mRotate.s);
			fh->Read(bone->mRotate.x);
			fh->Read(bone->mRotate.y);
			fh->Read(bone->mRotate.z);
			bone->mTranslate /= 100.0f;

			bone->mTransform = Matrix4::CreateRotation(bone->mRotate);
			bone->mTransform *= Matrix4::CreateTranslation(bone->mTranslate);
		}

		fh->Read(boneCount);
		int offset = mBoneList.Size();
		mBoneList.SetSize(offset + boneCount);
		for(unsigned int i = 0; i < boneCount; ++i){
			Bone* bone = &mBoneList[i + offset];
			bone->mDummy = true;
			bone->mName = fh->ReadTerminatedString();
			fh->Read(bone->mParentID);
			fh->Read(bone->mTranslate);
			bone->mTranslate /= 100.0f;
			bone->mTransform = Matrix4::IDENTITY;
			if(mVersion == 3){
				bone->mTransform = Matrix4::CreateRotation(bone->mRotate);
				fh->Read(bone->mRotate.s);
				fh->Read(bone->mRotate.x);
				fh->Read(bone->mRotate.y);
				fh->Read(bone->mRotate.z);
			}

			bone->mTransform *= Matrix4::CreateTranslation(bone->mTranslate);
		}

		TransformChildren(0);

		LOG("Loaded: '%s' B: %u", path, mBoneList.Size());

		fh->Close();
		return true;
	}

	void ZMD::TransformChildren(unsigned int parent){
		for(unsigned int i = 0; i < mBoneList.Size(); ++i){
			if(i == parent) continue;
			Bone* bone = &mBoneList[i];
			if(bone->mParentID != parent) continue;
			bone->mTransform *= mBoneList[bone->mParentID].mTransform;
			if(bone->mDummy) continue;
			TransformChildren(i);
		}
	}

	void ZMD::Render(){
		glPointSize(5.0f);
		for(unsigned int i = 0; i < mBoneList.Size(); ++i){
			Bone* bone = &mBoneList[i];
			Bone* parent = &mBoneList[bone->mParentID];

			Vector3 pos1 = bone->mTransform.TransformCoord(Vector3());
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_POINTS);
			glVertex3f(pos1.x, pos1.y, pos1.z);
			glEnd();

			if(bone == parent) continue;

			Vector3 pos2 = parent->mTransform.TransformCoord(Vector3());

			glColor3f(0.0f, 1.0f, 0.0f);
			glBegin(GL_LINES);
			glVertex3f(pos1.x, pos1.y, pos1.z);
			glVertex3f(pos2.x, pos2.y, pos2.z);
			glEnd();
		}
	}

	unsigned int ZMD::GetBoneCount() const {
		return mBoneList.Size();
	}

	ZMD::Bone* ZMD::GetBone(int id) const {
		return &mBoneList[id];
	}

	ZMD::Bone* ZMD::GetBone(const char* name) const {
		String tmp = name;
		for(unsigned int i = 0; i < mBoneList.Size(); ++i){
			if(mBoneList[i].mName == tmp) return &mBoneList[i];
		}

		return NULL;
	}

	unsigned int ZMD::GetBoneID(const char* name) const {
		String tmp = name;
		for(unsigned int i = 0; i < mBoneList.Size(); ++i){
			if(mBoneList[i].mName == tmp) return i;
		}

		return 0xFFFFFFFF;
	}

	const Array<ZMD::Bone>& ZMD::GetBoneList() const {
		return mBoneList;
	}
};
