#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Avatar.hpp"

#include "..\R3E\RoseData.hpp"

class Player : public Avatar {
public:
	Player(){
		mCharacterType = CHR_PLAYER;
		SetEntity(this);
	}

	virtual ~Player(){
	}
};

extern Player* gPlayer;

#endif
