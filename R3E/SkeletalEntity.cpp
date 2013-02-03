#include "Global.h"

#include "SkeletalEntity.hpp"

SkeletalEntity::SkeletalEntity()
	: mBoneMatrices(0), mBoneMatricesAbs(0), mCurFrame(0), mUpdateInterval(0), mPaused(false)
{
	mLastUpdate = clock();
	mType = ENTITY_1TEX_MESH;
}

SkeletalEntity::~SkeletalEntity(){
	SAFE_DELETE_ARRAY(mBoneMatrices);
	SAFE_DELETE_ARRAY(mBoneMatricesAbs);
}

void SkeletalEntity::AddChild(Entity* child){
	mChildren.PushBack(child);
	UpdateBoundingBox();
}

Entity* SkeletalEntity::SwapChild(Entity* orig, Entity* nchild){
	int idx = mChildren.Find(orig);
	if(idx == Array<Entity*>::NOT_FOUND) return NULL;
	UnbindEntity(orig);
	mChildren[idx] = nchild;
	UpdateBoundingBox();
	return orig;
}

Entity* SkeletalEntity::RemoveChild(Entity* child){
	int idx = mChildren.Find(child);
	if(idx == Array<Entity*>::NOT_FOUND) return NULL;
	UnbindEntity(child);
	mChildren.Erase(idx);
	UpdateBoundingBox();
	return child;
}

void SkeletalEntity::Update(){
	if(!mAnimation.Get()) return;

	if(clock() - mLastUpdate > mUpdateInterval){
		if(!mPaused)
			SetFrame(mCurFrame + 1);

		mLastUpdate = clock();
	}

	for(unsigned int i = 0; i < mBoundEntities.Size(); ++i){
		BoundEntity& bound = mBoundEntities[i];
		bound.mEntity->SetTransform(mBoneMatricesAbs[mCurFrame][bound.mBoneID]);
	}

	BoundingBox tmp = mBoundingBox;
	tmp.mMax += Vector3(0.0f, 0.0f, 0.25f);

	mBoundingBoxTransformed.Reset();
	mBoundingBoxTransformed.AddTransformedBox(tmp, mBoneMatrices[mCurFrame][0] * mTransform);
}

void SkeletalEntity::SetSkeleton(SmartPointer<ROSE::ZMD>& skel){
	mSkeleton = skel;
}

void SkeletalEntity::SetAnimation(SmartPointer<ROSE::ZMO>& anim){
	if(mAnimation == anim) return;
	mAnimation = anim;
	PreloadFrames();
}

void SkeletalEntity::PauseAnimation(){
	mPaused = true;
}

void SkeletalEntity::ResumeAnimation(){
	mPaused = false;
}

void SkeletalEntity::SetFrame(int frame){
	if(frame < 0) frame = 0;
	mCurFrame = frame;
	if(mCurFrame >= mAnimation->GetFrameCount()) mCurFrame = 0;
	Matrix4* mat = mBoneMatrices[mCurFrame].Data();
	for(unsigned int i = 0; i < mChildren.Size(); ++i)
		mChildren[i]->UpdateAnimation(mat);
}

unsigned int SkeletalEntity::GetBoneCount() const {
	if(!mBoneMatrices) return 0;
	return mBoneMatrices[0].Size();
}

void SkeletalEntity::BindEntityToBone(Entity* entity, const char* bone){
	unsigned int boneID = mSkeleton->GetBoneID(bone);
	if(boneID == 0xFFFFFFFF) return;
	BindEntityToBone(entity, boneID);
}

void SkeletalEntity::BindEntityToDummy(Entity* entity, int boneID){
	ROSE::ZMD* skeleton = mSkeleton.Get();
	if(!skeleton) return;
	const Array<ROSE::ZMD::Bone>& bindBoneList = skeleton->GetBoneList();
	for(unsigned int i = 0; i < bindBoneList.Size(); ++i){
		ROSE::ZMD::Bone* bone = &bindBoneList[i];
		if(!bone->mDummy) continue;
		boneID += i;
		break;
	}

	BindEntityToBone(entity, boneID);
}

void SkeletalEntity::BindEntityToBone(Entity* entity, int bone){
	BoundEntity bound;
	bound.mEntity = entity;
	bound.mBoneID = bone;
	mBoundEntities.PushBack(bound);
}

void SkeletalEntity::UnbindEntity(Entity* entity){
	if(entity->IsGroup()) UnbindEntityGroup((EntityGroup*)entity);
	for(unsigned int i = 0; i < mBoundEntities.Size(); ++i){
		BoundEntity& bound = mBoundEntities[i];
		if(bound.mEntity != entity) continue;
		mBoundEntities.Erase(i);
		return;
	}
}

void SkeletalEntity::UnbindEntityGroup(EntityGroup* group){
	for(unsigned int i = 0; i < group->mChildren.Size(); ++i){
		Entity* entity = group->mChildren[i];
		UnbindEntity(entity);
	}
}

void SkeletalEntity::PreloadFrames(){
	ROSE::ZMO* motion = mAnimation.Get();
	ROSE::ZMD* skeleton = mSkeleton.Get();

	if(!motion || !skeleton) return;

	SAFE_DELETE_ARRAY(mBoneMatrices);
	SAFE_DELETE_ARRAY(mBoneMatricesAbs);

	Array<Matrix4> frameMatrices;
	mCurFrame = 0;
	mUpdateInterval = 1000 / motion->GetFPS();

	mBoneMatrices = new Array<Matrix4>[motion->GetFrameCount()];
	mBoneMatricesAbs = new Array<Matrix4>[motion->GetFrameCount()];

	for(unsigned int f = 0; f < motion->GetFrameCount(); ++f){
		const Array<ROSE::ZMD::Bone>& bindBoneList = skeleton->GetBoneList();

		mBoneMatrices[f].SetSize(skeleton->GetBoneCount());
		mBoneMatricesAbs[f].SetSize(skeleton->GetBoneCount());
		motion->CreateBoneMatrices(f, bindBoneList, frameMatrices);

		for(unsigned int i = 0; i < bindBoneList.Size(); ++i){
			ROSE::ZMD::Bone* bone = &bindBoneList[i];
			mBoneMatrices[f][i] = Matrix4::CreateInverse(bone->mTransform) * frameMatrices[i];
			mBoneMatricesAbs[f][i] = frameMatrices[i];
		}
	}
}
