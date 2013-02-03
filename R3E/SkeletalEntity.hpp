#ifndef SKELETAL_ENTITY_H
#define SKELETAL_ENTITY_H

#include "EntityGroup.hpp"
#include "ZMD.hpp"
#include "ZMO.hpp"
#include "SmartPointer.hpp"
#include <time.h>

class SkeletalEntity : public EntityGroup {
private:
	struct BoundEntity {
		Entity* mEntity;
		int mBoneID;
	};

public:
	SkeletalEntity();
	virtual ~SkeletalEntity();

	virtual void Update();

	virtual void AddChild(Entity* child);
	virtual Entity* RemoveChild(Entity* child);
	virtual Entity* SwapChild(Entity* orig, Entity* nchild);

	void SetSkeleton(SmartPointer<ROSE::ZMD>& skel);
	void SetAnimation(SmartPointer<ROSE::ZMO>& anim);

	void PauseAnimation();
	void ResumeAnimation();
	void SetFrame(int frame);

	void BindEntityToBone(Entity* entity, int bone);
	void BindEntityToDummy(Entity* entity, int boneID);
	void BindEntityToBone(Entity* entity, const char* bone);

	void UnbindEntity(Entity* entity);
	void UnbindEntityGroup(EntityGroup* group);
	
	unsigned int GetBoneCount() const;

private:
	void PreloadFrames();

private:
	unsigned int mCurFrame;
	bool mPaused;

	clock_t mLastUpdate;
	clock_t mUpdateInterval;

	SmartPointer<ROSE::ZMD> mSkeleton;
	SmartPointer<ROSE::ZMO> mAnimation;

	Array<Matrix4>* mBoneMatrices;
	Array<Matrix4>* mBoneMatricesAbs;

	Array<BoundEntity> mBoundEntities;
};

#endif