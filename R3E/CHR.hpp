#ifndef ROSECHR_H
#define ROSECHR_H

#include "String.hpp"
#include "ZSC.hpp"

namespace ROSE {
	class CHR {
	public:
		struct Skeleton {
			String mPath;
		};

		struct Animation {
			String mPath;
		};

		struct Effect {
			String mPath;
		};

		struct ModelLink {
			short mModel;
		};

		struct AnimationLink {
			short mType;
			short mAnimation;
		};

		struct EffectLink {
			short mBoneIdx;
			short mEffect;
		};

		struct Character {
			unsigned char mActive;
			String mName;
			unsigned short mSkeleton;
			Array<ModelLink> mModelLinks;
			Array<AnimationLink> mAnimationLinks;
			Array<EffectLink> mEffectLinks;

			AnimationLink* GetAnimation(int type){
				for(unsigned int i = 0; i < mAnimationLinks.Size(); ++i)
					if(mAnimationLinks[i].mType == type)
						return &mAnimationLinks[i];

				return NULL;
			}
		};

	public:
		CHR();
		CHR(const char* path);

		~CHR();

		bool Load(const char* path);

		Effect* GetEffect(unsigned int id) const;
		Skeleton* GetSkeleton(unsigned int id) const;
		Animation* GetAnimation(unsigned int id) const;
		Character* GetCharacter(unsigned int id) const;

		unsigned int Effects() const;
		unsigned int Skeletons() const;
		unsigned int Animations() const;
		unsigned int Characters() const;

	public:
		Array<Skeleton> mSkeletonList;
		Array<Animation> mAnimationList;
		Array<Effect> mEffectList;
		Array<Character> mCharacterList;
	};
};

#endif
