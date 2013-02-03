#include "Global.h"

#include "CHR.hpp"
#include "Log.hpp"
#include "FileSystem.hpp"
#include "ScopedPointer.hpp"

namespace ROSE {
	CHR::CHR(){}
	CHR::CHR(const char* path){
		Load(path);
	}

	CHR::~CHR(){
		mSkeletonList.Delete();
		mAnimationList.Delete();
		mEffectList.Delete();
		mCharacterList.Delete();
	}

	bool CHR::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;

		unsigned short count, subcount;

		fh->Read(count);
		mSkeletonList.SetSize(count);
		for(unsigned short i = 0; i < count; ++i)
			mSkeletonList[i].mPath = fh->ReadTerminatedString();

		fh->Read(count);
		mAnimationList.SetSize(count);
		for(unsigned short i = 0; i < count; ++i)
			mAnimationList[i].mPath = fh->ReadTerminatedString();

		fh->Read(count);
		mEffectList.SetSize(count);
		for(unsigned short i = 0; i < count; ++i)
			mEffectList[i].mPath = fh->ReadTerminatedString();

		fh->Read(count);
		mCharacterList.SetSize(count);
		for(unsigned short i = 0; i < count; ++i){
			Character* character = &mCharacterList[i];
			fh->Read(character->mActive);
			if(!character->mActive) continue;
			
			fh->Read(character->mSkeleton);
			character->mName = fh->ReadTerminatedString();

			fh->Read(subcount);
			character->mModelLinks.SetSize(subcount);
			for(unsigned short i = 0; i < subcount; ++i)
				fh->Read(character->mModelLinks[i].mModel);

			fh->Read(subcount);
			character->mAnimationLinks.SetSize(subcount);
			for(unsigned short i = 0; i < subcount; ++i){
				fh->Read(character->mAnimationLinks[i].mType);
				fh->Read(character->mAnimationLinks[i].mAnimation);
			}

			fh->Read(subcount);
			character->mEffectLinks.SetSize(subcount);
			for(unsigned short i = 0; i < subcount; ++i){
				fh->Read(character->mEffectLinks[i].mBoneIdx);
				fh->Read(character->mEffectLinks[i].mEffect);
			}
		}

		LOG("Loaded: '%s' S: %u A: %u E: %u C: %u", path, mSkeletonList.Size(), mAnimationList.Size(), mEffectList.Size(), mCharacterList.Size());

		fh->Close();
		return true;
	}

	CHR::Skeleton* CHR::GetSkeleton(unsigned int id) const {
		if(id >= mSkeletonList.Size()) return NULL;
		return &mSkeletonList[id];
	}

	CHR::Animation* CHR::GetAnimation(unsigned int id) const {
		if(id >= mAnimationList.Size()) return NULL;
		return &mAnimationList[id];
	}

	CHR::Effect* CHR::GetEffect(unsigned int id) const {
		if(id >= mEffectList.Size()) return NULL;
		return &mEffectList[id];
	}

	CHR::Character* CHR::GetCharacter(unsigned int id) const {
		if(id >= mCharacterList.Size()) return NULL;
		return &mCharacterList[id];
	}

	unsigned int CHR::Effects() const {
		return mEffectList.Size();
	}

	unsigned int CHR::Skeletons() const {
		return mSkeletonList.Size();
	}

	unsigned int CHR::Animations() const {
		return mAnimationList.Size();
	}

	unsigned int CHR::Characters() const {
		return mCharacterList.Size();
	}
};