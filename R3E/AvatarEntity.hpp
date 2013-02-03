#ifndef AVATAR_ENTITY_HPP
#define AVATAR_ENTITY_HPP

#include "SkeletalEntity.hpp"

class AvatarEntity : public SkeletalEntity {
public:
	AvatarEntity();
	virtual ~AvatarEntity();

	void SetGender(int gender);
	bool SetItem(int type, int id);

	void SetAvatarState(int state);
	void SetHairStyle(int style);
	void SetFaceStyle(int style);

	int GetAvatarState();

private:
	void UpdateHair();
	void UpdateAnimationType();

protected:
	Array<Entity*> mItemEntities;
	int mGender;
	int mHairStyle;
	int mAvatarState;
};

#endif