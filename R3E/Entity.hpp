#ifndef ENTITY_H
#define ENTITY_H

#include "BoundingBox.hpp"
#include "Matrix4.hpp"
#include "Frustum.hpp"
#include "Ray.hpp"

enum EntityType {
	ENTITY_COLOUR,
	ENTITY_COLOUR_MESH,
	ENTITY_1TEX_SPRITE,
	ENTITY_1TEX_MESH,
	ENTITY_2TEX_OBJECT,
	ENTITY_3TEX_TERRAIN,
	MAX_ENTITY,
};

enum EntityFlagIndex {
	ENTITY_CAN_HOVER_IDX,
	ENTITY_CAN_CLICK_IDX,
	ENTITY_CAN_MOVE_IDX,
	ENTITY_CAN_UPDATE_IDX,
	ENTITY_CAN_RAYMESH_IDX,
	ENTITY_FLAG_COUNT,
};

enum EntityFlagBit {
	ENTITY_NO_FLAGS,
	ENTITY_CAN_HOVER = 1,
	ENTITY_CAN_CLICK = 1 << 1,
	ENTITY_CAN_MOVE = 1 << 2,
	ENTITY_CAN_UPDATE = 1 << 3,
	ENTITY_CAN_RAYMESH = 1 << 4,
};

class Entity {
public:
	Entity() : mVisible(true), mType(MAX_ENTITY), mHasTransform(false), mID(0), mFlags(ENTITY_NO_FLAGS) {}
	virtual ~Entity(){}

	virtual int UpdateCulling(const Frustum& camf){
		int ret = mBoundingBoxTransformed.CheckIntersectFrustum(camf);
		mVisible = (ret != 0);
		return ret;
	}

	virtual void Render(){}
	virtual void Update(){}
	
	virtual void UpdateAnimation(Matrix4* /*mat*/){}

	virtual bool IsSkinned(){
		return false;
	}

	virtual bool IsGroup(){
		return false;
	}

	virtual bool CastRay(const Ray& ray, Vector3& out, float maxLengthSq){
		if(!ray.IntersectsBox(mBoundingBoxTransformed)) return false;
		return true;
	}

	virtual bool CastRayDown(const Ray& ray, Vector3& out){
		if(!mBoundingBoxTransformed.Contains2D(ray.mPoint)) return false;
		return true;
	}

	virtual void UpdateBoundingBox(){
		mBoundingBoxTransformed.Reset();
		mBoundingBoxTransformed.AddTransformedBox(mBoundingBox, mTransform);
	}

	void Transform(const Matrix4& mult){
		mTransform *= mult;
		UpdateBoundingBox();
		mHasTransform = true;
	}

	void SetTransform(const Matrix4& mat){
		mTransform = mat;
		UpdateBoundingBox();
		mHasTransform = true;
	}

	void SetBoundingBox(const BoundingBox& box){
		mBoundingBox = box;
		UpdateBoundingBox();
		mHasTransform = true;
	}

	const Matrix4& GetTransform(){
		return mTransform;
	}

	Vector3 GetEntityPosition(){
		return mTransform.GetTranslate();
	}

	float GetEntityHeight(){
		return mBoundingBoxTransformed.mMax.z - mBoundingBoxTransformed.mMin.z;
	}

	const BoundingBox& GetBoundingBox(){
		return mBoundingBoxTransformed;
	}

	void SetID(int id){
		mID = id;
	}

public:
	bool mVisible;
	int mType;
	int mID;
	int mFlags;

	bool mHasTransform;

	Matrix4 mTransform;
	BoundingBox mBoundingBox;
	BoundingBox mBoundingBoxTransformed;

	friend class SceneManager;
	friend class EntityGroup;
};

#endif