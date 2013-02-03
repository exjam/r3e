#ifndef AVATAR_HPP
#define AVATAR_HPP

#include "String.hpp"
#include "Character.hpp"
#include "StateManager.hpp"

#include "..\R3E\AvatarEntity.hpp"

#include <time.h>

class Label;

class Avatar : public Character, public AvatarEntity {
public:
	Avatar();
	virtual ~Avatar();

	virtual void Update();

	void SetName(const char* name);
	void SetItems(struct tagPartItem* items);

	virtual void Draw2D();
	virtual void OnStateChange();
	virtual const char* GetName();

protected:
	String mName;
	Label* mLabelName;
};

#endif
