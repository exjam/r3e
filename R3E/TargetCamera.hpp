#ifndef TARGET_CAMERA_H
#define TARGET_CAMERA_H

#include "Camera.hpp"
#include "Entity.hpp"

class TargetCamera : public Camera {
public:
	TargetCamera(){
		mEye = Vector3(0.0f);
		mUp = Vector3(0.0f, 0.0f, 1.0f);

		mTarget = Vector3(0.0f, 0.0f, 1.0f);
		mDistance = 3.0f;
		mVRotate = float(M_PI_2);
		mHRotate = float(M_PI) * 1.5f;
		mHeight = 1.7f;

		mHasChanges = true;

		mFollow = NULL;
	}

	virtual ~TargetCamera(){}

	void FollowEntity(Entity* target){
		mFollow = target;
		CheckFollowChanges();
	}

	void SetTarget(Vector3 target){
		mTarget = target;
		mFollow = NULL;
		mHasChanges = true;
	}

	void SetDistance(float dist){
		mDistance = dist;
		mHasChanges = true;
	}

	void SetHRotate(float rotate){
		mHRotate = rotate;
		mHasChanges = true;
	}

	void SetVRotate(float rotate){
		mVRotate = rotate;
		mHasChanges = true;
	}

	virtual bool UpdateTransform(){
		CheckFollowChanges();
		if(!mHasChanges) return false;
		UpdateEye();
		mTransform = Matrix4::CreateLookAtRH(mEye, mTarget, mUp);
		mHasChanges = false;
		return true;
	}

	virtual void ApplyProjection(){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		OpenGL::Perspective(45.0f, float(mViewWidth) / float(mViewHeight), 1.0f, 1050.0f);
		UpdateProjectionMatrix();

		glMatrixMode(GL_MODELVIEW);
	}

	virtual const Vector3& GetCameraPosition(){
		return mEye;
	}

	void AddDistance(float dist){
		mDistance += dist;
		if(mDistance < 1.0f) mDistance = 1.0f;
		mHasChanges = true;
	}

	void MultDistance(float dist){
		mDistance *= dist;
		if(mDistance < 1.0f) mDistance = 1.0f;
		mHasChanges = true;
	}

	void RotateV(float amount){
		mVRotate += amount;
		if(mVRotate < 0.1f) mVRotate = 0.1f;
		if(mVRotate > 3.10f) mVRotate = 3.10f;
		mHasChanges = true;
	}

	void RotateH(float amount){
		mHRotate += amount;
		mHasChanges = true;
	}

	float GetRotateH(){
		return mHRotate;
	}

	float GetRotateV(){
		return mVRotate;
	}

private:
	void CheckFollowChanges(){
		if(!mFollow) return;
		Vector3 trans = mFollow->GetTransform().GetTranslate();
		if(trans == mTarget) return;
		mTarget = trans;
		mTarget.z += mHeight;
		mHasChanges = true;
	}

	void UpdateEye(){
		mEye.x = mTarget.x + (mDistance * sin(mVRotate) * cos(mHRotate));
		mEye.y = mTarget.y + (mDistance * sin(mVRotate) * sin(mHRotate));
		mEye.z = mTarget.z + (mDistance * cos(mVRotate));
	}

private:
	bool mHasChanges;

	Vector3 mEye;
	Vector3 mTarget;
	Vector3 mUp;

	float mHeight;
	float mDistance;
	float mHRotate;
	float mVRotate;

	Entity* mFollow;
};

#endif