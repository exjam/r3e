#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix4.hpp"
#include "Frustum.hpp"

class Camera {
public:
	Camera(){}
	virtual ~Camera(){}

	virtual bool UpdateTransform() = 0;
	virtual void ApplyProjection() = 0;
	virtual const Vector3& GetCameraPosition() = 0;

	bool Apply(){
		if(!UpdateTransform()) return false;
		mTransform.glLoad();
		mFinalMatrix = mTransform * mProjectionMatrix;
		mFrustum = Frustum::CreateFromMatrix(mFinalMatrix);
		return true;
	}

	const Frustum& GetFrustum() const {
		return mFrustum;
	}

	void UpdateProjectionMatrix(){
		glGetFloatv(GL_PROJECTION_MATRIX, &mProjectionMatrix.m[0][0]);
		mFinalMatrix = mTransform * mProjectionMatrix;
		mFrustum = Frustum::CreateFromMatrix(mFinalMatrix);
	}

	const Matrix4& GetTransform(){
		return mTransform;
	}

	const Matrix4& GetProjection(){
		return mProjectionMatrix;
	}

	const Matrix4& GetFinalMatrix(){
		return mFinalMatrix;
	}

	void SetViewport(int width, int height){
		mViewWidth = width;
		mViewHeight = height;
		glViewport(0, 0, width, height);
	}
	
protected:
	Matrix4 mTransform;
	Matrix4 mProjectionMatrix;
	Matrix4 mFinalMatrix;
	Frustum mFrustum;

	int mViewWidth;
	int mViewHeight;
};

#endif