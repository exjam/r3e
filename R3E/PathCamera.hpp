#ifndef PATH_CAMERA_HPP
#define PATH_CAMERA_HPP

#include "Camera.hpp"
#include "Array.hpp"
#include "ZMO.hpp"
#include <time.h>

class PathCamera : public Camera {
private:
	struct FovyNearFarChan {
		float fovy;//*100.0f
		float vnear;
		float vfar;
	};

public:
	static const unsigned int REPEAT_INFINITE = 0xFFFFFFFF;

	PathCamera(){}
	virtual ~PathCamera(){}

	void SetPath(SmartPointer<ROSE::ZMO>& path){
		mMatrixFrames.Clear();

		mPath = path;
		mPath->CreateCameraMatrices(mMatrixFrames);
		mFovyNearFar = (FovyNearFarChan*)mPath->GetChannel(3)->mData;

		mCurrentFrame = 0;
		mLastFrame = clock();
		mFrameInterval = 1000 / mPath->GetFPS();
		mTransform = mMatrixFrames[mCurrentFrame];
		mTranslate = mTransform.GetTranslate();
	}

	void SetRepeatCount(int count){
		mRepeatCount = count;
	}

	bool IsFinished(){
		return (mRepeatCount == 0) && (mCurrentFrame == mMatrixFrames.Size() - 1);
	}

	virtual const Vector3& GetCameraPosition(){
		return mTranslate;
	}

	virtual void ApplyProjection(){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		OpenGL::Perspective(mFovyNearFar[mCurrentFrame].fovy * 100.0f, float(mViewWidth) / float(mViewHeight),
			mFovyNearFar[mCurrentFrame].vnear, mFovyNearFar[mCurrentFrame].vfar);

		UpdateProjectionMatrix();

		glMatrixMode(GL_MODELVIEW);
	}

	virtual bool UpdateTransform(){
		if(mRepeatCount == 0) return false;
		if((clock() - mLastFrame) < mFrameInterval) return false;
		
		++mCurrentFrame;
		if(mCurrentFrame >= mMatrixFrames.Size()){
			mCurrentFrame = 0;
			--mRepeatCount;
			if(!mRepeatCount)
				mCurrentFrame = mMatrixFrames.Size() - 1;
		}

		mTransform = mMatrixFrames[mCurrentFrame];
		mTranslate = ((Vector3*)(mPath->GetChannel(0)->mData))[mCurrentFrame];
		mTranslate += Vector3(5200.0f, 5200.0f, 0.0f);

		ApplyProjection();

		mLastFrame = clock();
		return true;
	}

private:
	SmartPointer<ROSE::ZMO> mPath;

	clock_t mLastFrame;
	clock_t mFrameInterval;
	unsigned int mCurrentFrame;
	int mRepeatCount;
	Vector3 mTranslate;
	Array<Matrix4> mMatrixFrames;
	FovyNearFarChan* mFovyNearFar;
};

#endif