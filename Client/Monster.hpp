#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "Fonts.hpp"
#include "Character.hpp"

#include "..\RGE\Label.hpp"
#include "..\R3E\NpcEntity.hpp"
#include "..\R3E\NpcManager.hpp"
#include "..\R3E\SceneManager.hpp"

class Monster : public Character, public NpcEntity {
public:
	Monster(){
		mCharacterType = CHR_MONSTER;
		SetEntity(this);

		mLabelName = new Label();
		mLabelName->SetFont(FONT_NORMAL_OUTLINE);
		mLabelName->SetAlign(ALIGN_CENTER);
		mLabelName->SetAutosize(true);
	}

	virtual ~Monster(){
		SAFE_DELETE(mLabelName);
	}

	virtual void Update(){
		NpcEntity::Update();
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
			//mLabelName->SetDepth(vpos.z);
		}else
			mLabelName->SetVisible(false);
	}

	virtual void Draw2D(){
		mLabelName->Render();
	}

	virtual void OnStateChange(){
	}

	virtual const char* GetName(){
		return NPC_MAN().GetName(mNpcID);
	}

	bool SetNpcID(int id){
		mLabelName->SetText(NPC_MAN().GetName(id));
		SetScale(float(NPC_MAN().GetScale(id)) / 100.0f);
		return NpcEntity::SetNpcID(id);
	}

private:
	Label* mLabelName;
};

#endif
