#include "Global.h"

#include "ZMO.hpp"
#include "Log.hpp"
#include "FileSystem.hpp"
#include "ScopedPointer.hpp"

namespace ROSE {
	ZMO::ZMO(){}
	ZMO::~ZMO(){
		mChannels.Clear();
	}

	void ZMO::Unload(){
		mChannels.Clear();
	}

	bool ZMO::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;

		char VersionHeader[8];
		fh->ReadData(VersionHeader, 8);
		if(strncmp(VersionHeader, "ZMO0002", 7)){
			LOG("Error: '%s' invalid version '%s'", path, VersionHeader);
			fh->Close();
			return false;
		}

		unsigned int channels;
		fh->Read(mFPS);
		fh->Read(mFrameCount);
		fh->Read(channels);

		mChannels.SetSize(channels);
		for(unsigned int i = 0; i < channels; ++i){
			Channel* chan = &mChannels[i];
			fh->Read(chan->mType);
			fh->Read(chan->mBoneID);
			if(chan->mType == CTYPE_POSITION) chan->mData = (char*)new Vector3[mFrameCount];
			else if(chan->mType == CTYPE_ROTATION) chan->mData = (char*)new Quaternion[mFrameCount];
		}

		for(unsigned int i = 0; i < mFrameCount; ++i){
			for(unsigned int j = 0; j < channels; ++j){
				Channel* chan = &mChannels[j];
				if(chan->mType == CTYPE_POSITION){
					Vector3* pos = (Vector3*)chan->mData;
					fh->Read(pos[i].x);
					fh->Read(pos[i].y);
					fh->Read(pos[i].z);
					pos[i] /= 100.0f;
				}else if(chan->mType == CTYPE_ROTATION){
					Quaternion* rot = (Quaternion*)chan->mData;
					fh->Read(rot[i].s);
					fh->Read(rot[i].x);
					fh->Read(rot[i].y);
					fh->Read(rot[i].z);
				}
			}
		}

		LOG("Loaded: '%s' C: %u F: %u", path, mChannels.Size(), mFrameCount);

		fh->Close();
		return true;
	}

	ZMO::Channel* ZMO::GetChannel(int idx) const {
		return &mChannels[idx];
	}

	ZMO::Channel* ZMO::GetBoneChannel(int bone, int type) const {
		for(unsigned int i = 0; i < mChannels.Size(); ++i){
			Channel* chan = &mChannels[i];
			if(chan->mBoneID == bone && chan->mType == type) return chan;
		}

		return NULL;
	}

	unsigned int ZMO::GetFrameCount() const {
		return mFrameCount;
	}

	unsigned int ZMO::GetFPS() const {
		return mFPS;
	}

	void ZMO::CreateBoneMatrices(int frame, const Array<ZMD::Bone>& bindBoneList, Array<Matrix4>& matrices) const {
		matrices.SetSize(bindBoneList.Size());
		for(unsigned int i = 0; i < bindBoneList.Size(); ++i){
			ZMD::Bone* bone = &bindBoneList[i];
			Channel* posC = GetBoneChannel(i, CTYPE_POSITION);
			Channel* rotC = GetBoneChannel(i, CTYPE_ROTATION);

			matrices[i] = Matrix4::IDENTITY;

			if(rotC) matrices[i] = Matrix4::CreateRotation(((Quaternion*)rotC->mData)[frame]);
			else matrices[i] = Matrix4::CreateRotation(bone->mRotate);
			
			if(posC) matrices[i] *= Matrix4::CreateTranslation(((Vector3*)posC->mData)[frame]);
			else matrices[i] *= Matrix4::CreateTranslation(bone->mTranslate);
		}

		TransformChildren(bindBoneList, matrices, 0);
	}

	void ZMO::CreateCameraMatrices(Array<Matrix4>& matrices) const {
		Vector3* eye = (Vector3*)mChannels[0].mData;
		Vector3* target = (Vector3*)mChannels[1].mData;
		Vector3* up = (Vector3*)mChannels[2].mData;
		Vector3 trans = Vector3(5200.0f, 5200.0f, 0.0f);

		matrices.SetSize(mFrameCount);
		for(unsigned int i = 0; i < mFrameCount; ++i){
			Vector3 e = eye[i] + trans;
			Vector3 t = target[i] + trans;
			Vector3 u = up[i] * 100.0f;
			u.Normalise();

			matrices[i] = Matrix4::CreateLookAtRH(e, t, up[i]);
		}
	}

	void ZMO::TransformChildren(const Array<ZMD::Bone>& bindBoneList, Array<Matrix4>& matrices, unsigned int parent) const {
		for(unsigned int i = 0; i < bindBoneList.Size(); ++i){
			if(i == parent) continue;
			ZMD::Bone* bone = &bindBoneList[i];
			if(bone->mParentID != parent) continue;
			matrices[i] *= matrices[bone->mParentID];
			if(bone->mDummy) continue;
			TransformChildren(bindBoneList, matrices, i);
		}
	}
};
