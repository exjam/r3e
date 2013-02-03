#include "Global.h"

#include "Fonts.hpp"
#include "Avatar.hpp"
#include "RoseStructs.hpp"
#include "RoseConstants.hpp"

#include "..\RGE\Label.hpp"
#include "..\R3E\SceneManager.hpp"

Avatar::Avatar(){
	mCharacterType = CHR_AVATAR;
	SetEntity(this);

	mLabelName = new Label();
	mLabelName->SetFont(FONT_NORMAL_OUTLINE);
	mLabelName->SetAlign(ALIGN_CENTER);
	mLabelName->SetAutosize(true);
}

Avatar::~Avatar(){
	SAFE_DELETE(mLabelName);
}

void Avatar::Update(){
	AvatarEntity::Update();
	Character::Update();

	if(mVisible){
		Vector3 wpos = GetPosition();
		wpos.z += GetEntityHeight();
		Vector3 vpos = gScene->GetScreenPosition(wpos);
		Point pos(int(vpos.x), int(vpos.y));
		pos.x -= mLabelName->SizeX() / 2;
		pos.y -= mLabelName->SizeY();
		mLabelName->SetPosition(pos);
		mLabelName->SetVisible(true);
		mLabelName->SetDepth(1.0f / vpos.z);
	}else
		mLabelName->SetVisible(false);
}

void Avatar::Draw2D(){
	mLabelName->Render();
}

void Avatar::SetItems(tagPartItem* items){
	SetItem(IT_CAP, items[BP_HELMET].mItemNo);
	SetItem(IT_BODY, items[BP_ARMOR].mItemNo);
	SetItem(IT_ARM, items[BP_GLOVES].mItemNo);
	SetItem(IT_FOOT, items[BP_BOOTS].mItemNo);
	SetItem(IT_MASK, items[BP_FACEITEM].mItemNo);
	SetItem(IT_BACK, items[BP_BACKITEM].mItemNo);
	SetItem(IT_WEAPON, items[BP_WEAPON_R].mItemNo);
	SetItem(IT_SUBWPN, items[BP_WEAPON_L].mItemNo);
}

void Avatar::SetName(const char* name){
	mName = name;
	mLabelName->SetText(name);
}

const char* Avatar::GetName(){
	return mName;
}

void Avatar::OnStateChange(){
	static const unsigned int map[] = { AVT_ANI_STOP1, AVT_ANI_WALK, AVT_ANI_RUN };
	SetAvatarState(map[mState]);
}
