#include "Global.h"

#include "Character.hpp"
#include "StateManager.hpp"
#include "..\R3E\Entity.hpp"
#include "..\R3E\SceneManager.hpp"

Character::Character(){
	mMoveSpeed = 9.6f;
	mMoveTime = 0;
	mDirection = 0.0f;
	mScale = 1.0f;
	mState = STATE_STOP;
}

Character::~Character(){}

#define GRAVITY_PER_MS 0.01f
#define MAX_CHAR_STEP_HEIGHT 1.0f

void Character::Update(){
	if(mMoveTime){
		if(mState == STATE_STOP)
			SetState(STATE_RUN);

		Vector3 nextPos = mPosition;
		Vector3 delta = mTargetPosition - mPosition;
		float dist = delta.Length2D();
		clock_t etime = clock() - mMoveTime;
		float ntime = (dist / mMoveSpeed) * 1000;

		if(ntime <= etime || dist < 0.001f){
			mMoveTime = 0;
			nextPos.x = mTargetPosition.x;
			nextPos.y = mTargetPosition.y;
			SetState(STATE_STOP);
		}else{
			float dt = etime / ntime;
			nextPos.x = delta.x*dt + mPosition.x;
			nextPos.y = delta.y*dt + mPosition.y;
			mMoveTime = clock();
		}

		bool hasStopped = false;
		if(mCharacterType == CHR_PLAYER){
			Ray forwardRay;
			Vector3 forwardPos;
			forwardRay.mPoint = mPosition;
			forwardRay.mPoint.z += MAX_CHAR_STEP_HEIGHT;
			forwardRay.mDirection = delta;
			forwardRay.mDirection.Normalise();

			if(gScene->CastRay(forwardRay, forwardPos, 5.0f)
				&& (forwardPos.z > (nextPos.z + MAX_CHAR_STEP_HEIGHT))
				&& (forwardPos.Distance2Dsq(nextPos) < mPosition.Distance2Dsq(nextPos)))
			{
				float dZ = forwardPos.z - mPosition.z;
				float gradient = (dZ * dZ) / mPosition.Distance2Dsq(forwardPos);
				if(gradient > 0.55f){
					hasStopped = true;
					mMoveTime = 0;
					SetState(STATE_STOP);
				}
			}
		}

		if(!hasStopped){
			Vector3 downPos;

			Ray downRay;
			downRay.mPoint = nextPos;
			downRay.mPoint.z += MAX_CHAR_STEP_HEIGHT;
			downRay.mDirection = Vector3(0.0f, 0.0f, -1.0f);

			float zWithGrav = nextPos.z - (GRAVITY_PER_MS * etime);

			if(gScene->CastRayDown(downRay, downPos) && downPos.z > zWithGrav){
				if(mCharacterType == CHR_PLAYER){
					float dZ = downPos.z - nextPos.z;
					float gradient = (dZ * dZ) / mPosition.Distance2Dsq(nextPos);
					if(gradient > 0.55f && dZ > 0.5f){
						hasStopped = true;
						mMoveTime = 0;
						SetState(STATE_STOP);
					}
				}

				if(!hasStopped)
					nextPos.z = downPos.z;
			}else{
				nextPos.z = zWithGrav;
			}

			mPosition = nextPos;
		}

		UpdateTransform();
	}
}
void Character::MoveTo(Vector2& pos){
	mTargetPosition.x = pos.x;
	mTargetPosition.y = pos.y;
	mMoveTime = clock();

	Vector3 dir = mTargetPosition - mPosition;
	mDirection = atan2(dir.y, dir.x) + float(M_PI_2);
}

void Character::MoveTo(Vector3& pos){
	MoveTo(Vector2(pos.x, pos.y));
}

void Character::SetMoveSpeed(short mspeed){
	mMoveSpeed = mspeed;
	mMoveSpeed /= 100.0f;
}

void Character::SetScale(float scale){
	mScale = scale;
	UpdateTransform();
}

void Character::SetDirection(float dir){
	mDirection = dir;
	UpdateTransform();
}

void Character::SetPosition(const Vector3& pos){
	mMoveTime = 0;

	mPosition = pos;
	Vector3 tmpPos = mPosition;
	tmpPos.z += 1.0f;

	Ray downRay;
	downRay.mPoint = tmpPos;
	downRay.mDirection = Vector3(0.0f, 0.0f, -1.0f);
	if(gScene->CastRayDown(downRay, tmpPos))
		mPosition = tmpPos;

	mTargetPosition = mPosition;

	UpdateTransform();
}

void Character::SetPosition(const Vector2& pos){
	SetPosition(Vector3(pos.x, pos.y, 0.0f));
}

void Character::SetClientID(unsigned int cid){
	mClientID = cid;
}

void Character::SetEntity(Entity* entity){
	mEntity = entity;
}

void Character::SetState(int state){
	mState = state;
	OnStateChange();
}

const Vector3& Character::GetPosition() const {
	return mPosition;
}

unsigned int Character::GetClientID() const {
	return mClientID;
}

unsigned short Character::GetCharacterType() const {
	return mCharacterType;
}

void Character::UpdateTransform(){
	mEntity->SetTransform(Matrix4::CreateRotationZ(mDirection) * Matrix4::CreateScaling(Vector3(mScale)) * Matrix4::CreateTranslation(mPosition));
}
