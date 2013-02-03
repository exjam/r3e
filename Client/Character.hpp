#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <time.h>
#include "..\R3E\Vector3.hpp"

enum CharacterType {
	CHR_AVATAR,
	CHR_NPC,
	CHR_MONSTER,
	CHR_PLAYER,
};

enum CharacterState {
	STATE_STOP,
	STATE_WALK,
	STATE_RUN,
};

class Entity;

class Character {
public:
	Character();
	~Character();

	void Update();

	void SetState(int state);
	void SetScale(float scale);
	void SetDirection(float dir);
	void SetMoveSpeed(short mspeed);
	void SetEntity(Entity* entity);
	void SetClientID(unsigned int cid);
	void SetPosition(const Vector3& pos);
	void SetPosition(const Vector2& pos);

	void MoveTo(Vector2& pos);
	void MoveTo(Vector3& pos);

	unsigned int GetClientID() const;
	const Vector3& GetPosition() const;
	unsigned short GetCharacterType() const;

	virtual void Draw2D() = 0;
	virtual void OnStateChange() = 0;
	virtual const char* GetName() = 0;

private:
	void UpdateTransform();

private:
	Entity* mEntity;

protected:
	unsigned int mState;

	unsigned int mClientID;
	unsigned int mTargetID;
	unsigned short mCharacterType;

	float mScale;
	float mDirection;

	Vector3 mPosition;
	Vector3 mTargetPosition;

	float mMoveSpeed;
	clock_t mMoveTime;
};

#endif
