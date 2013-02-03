#ifndef ROSE_STRUCTS_HPP
#define ROSE_STRUCTS_HPP

#include "..\R3E\Vector2.hpp"

#pragma pack(push, 1)

struct tagPartItem {
	unsigned int mItemNo : 10;
	unsigned int mStat : 9;
	unsigned int mHasSocket : 1;
	unsigned int mGrade : 4;
};

struct tagBasicInfo {
	char mBirthStone;
	char mFaceIDX;
	char mHairIDX;
	short mClass;
	char mUnion;
	char mRank;
	char mFame;
};

struct tagBasicAbility {
	short mStrength;
	short mDexterity;
	short mIntelligence;
	short mConcentration;
	short mCharm;
	short mSense;
};

struct tagMaintainStatus {
	unsigned long mExpiredSEC;
	short mValue;
	short mPadding;
};

struct tagGrowAbility {
	short mHP;
	short mMP;
	long mEXP;
	short mLevel;
	short mBonusPoint;
	short mSkillPoint;
	unsigned char mBodySIZE;
	unsigned char mHeadSIZE;
	long mPenaltyEXP;
	short mFameG;
	short mFameB;
	short mUnionPoints[10];
	int mPadding1;
	short mPadding2;
	char mPadding3;
	short mPKFlag;
	short mStamina;
	tagMaintainStatus mStatus[40];
};

struct tagSkillAbility {
	union {
		short mSkills[120];
		short mSkillPages[4][30];
	};
};

struct tagHotIcon {
	unsigned short mType : 5;
	unsigned short mSlot : 11;
};

struct tagSkillBar {
	union {
		tagHotIcon mSkills[32];
		tagHotIcon mSkillPages[4][8];
	};
};

struct pakPlayerData {
	char mGender;
	short mZone;
	Vector2 mPosition;
	short mReviveZone;
	tagPartItem mEquipment[10];
	tagBasicInfo mBasicInfo;
	tagBasicAbility mAttributes;
	tagGrowAbility mGrowAbility;
	tagSkillAbility mSkills;
	tagSkillBar mSkillBar;
	unsigned long mUniqueTAG;
};

enum AddCharCommands {
	CMD_STOP = 0,
	CMD_MOVE = 1,
	CMD_ATTACK = 2,
	CMD_DIE = 3,
	CMD_PICK_ITEM = 4,
	CMD_SKILL2SELF = 6,
	CMD_SKILL2OBJ = 7,
	CMD_SKILL2POS = 8,
	CMD_RUNAWAY = 0x8009,//why is it | with 0x8000 i dont know..
	CMD_SIT = 10,
};

struct tagAddChar {
	unsigned short mClientID;
	Vector2 mPosition;
	Vector2 mTargetPosition;
	unsigned short mCommand;
	unsigned short mTargetID;
	unsigned char mMoveMode;
	int mHP;
	int mTeamID;
	unsigned long mStatusFlag;
};

struct tagShotData {
	unsigned short mType : 5;
	unsigned short mItem : 10;
};

enum pakSpawnPlayer_subFlag {
	PSF_HIDE       = 0x00000001,
	PSF_SHOP       = 0x00000002,
	PSF_INTRO_CHAT = 0x00000004,
	PSF_ARUA_FAIRY = 0x40000000,
};

struct tagShopID {
	short mType;
	String mTitle;
};

struct tagClanID {
	unsigned long mID;
	unsigned short mMarkBack;
	unsigned short mMarkFront;
	unsigned char mLevel;
	unsigned char mRank;
	String mName;
};

struct pakSpawnPlayerData : public tagAddChar {
	unsigned char mGender;
	short mMoveSpeed;
	short mAttackSpeed;
	unsigned char mWeightRate;
	tagPartItem mEquipment[10];
	tagShotData mBullets[3];
	short mJob;
	unsigned char mLevel;
	tagPartItem mPAT[4];
	short mPosZ;
	unsigned long mSubFlag;
};

struct pakSpawnMonsterData : public tagAddChar {
	short mCharacterIndex;
	short mQuestIndex;
};

struct pakSpawnNPCData : public pakSpawnMonsterData {
	float mModelDirection;
	short mEventStatus;
};

enum DamageBits {
	DMG_BIT_DUMMY = 0x0800,
	DMG_BIT_IMMEDIATE = 0x1000,
	DMG_BIT_HITTED = 0x2000,
	DMG_BIT_CRITICAL = 0x4000,
	DMG_BIT_DEAD = 0x8000,
};

enum DamageActions {
	DMG_ACT_RESERVED = 0x01,
	DMG_ACT_ATTACK = 0x02,
	DMG_ACT_HITTED = 0x04,
	DMG_ACT_CRITICAL = 0x08,
	DMG_ACT_DEAD = 0x10,
};

struct pakDamageData {
	unsigned short mAttackerCID;
	unsigned short mDefenderCID;
	union {
		unsigned short mDamage;
		struct {
			unsigned short mValue  : 11;
			unsigned short mAction :  5;
		};
	};
	//tagdropitem can be here!
};

#pragma pack(pop)

#endif
