#ifndef NPC_HPP
#define NPC_HPP

#include "Character.hpp"

#include "..\R3E\NpcEntity.hpp"
#include "..\R3E\NpcManager.hpp"

class Npc : public Character, public NpcEntity {
public:
	Npc(){
		mCharacterType = CHR_NPC;
		SetEntity(this);
	}

	virtual ~Npc(){}

	virtual void Update(){
		NpcEntity::Update();
		Character::Update();
	}

	virtual void Draw2D(){
	}

	virtual void OnStateChange(){
	}

	virtual const char* GetName(){
		return "NPC";
	}
};

#endif
