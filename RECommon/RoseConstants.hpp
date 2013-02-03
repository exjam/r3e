#ifndef CONSTANTS_H
#define CONSTANTS_H

enum ItemType {
	IT_MASK = 1,
	IT_CAP = 2,
	IT_BODY = 3,
	IT_ARM = 4,
	IT_FOOT = 5,
	IT_BACK = 6,
	IT_JEWEL = 7,
	IT_WEAPON = 8,
	IT_SUBWPN = 9,
	IT_USE = 10,
	IT_GEM = 11,
	IT_NATURAL = 12,
	IT_QUEST = 13,
	IT_PAT = 14,
	IT_FACE = 15,
	IT_HAIR = 16,
	IT_MAX
};

enum BodyParts {
	BP_FACE = 0,
	BP_HAIR = 1,
	BP_HELMET = 2,
	BP_ARMOR = 3,
	BP_GLOVES = 4,
	BP_BOOTS = 5,
	BP_FACEITEM = 6,
	BP_BACKITEM = 7,
	BP_WEAPON_R = 8,
	BP_WEAPON_L = 9,
	MAX_BODY_PART = 10,
};

enum Gender {
	G_MALE = 0,
	G_FEMALE = 1,
	G_MAX
};

enum Pat {
	PAT_CART = 21,
	PAT_CASTLE_GEAR = 31,
	PAT_MAX,
};

enum MobMotionType {
	MOB_ANI_STOP = 0,
	MOB_ANI_MOVE,
	MOB_ANI_ATTACK,
	MOB_ANI_HIT,
	MOB_ANI_DIE,
	MOB_ANI_RUN,
	MOB_ANI_CAST1,
	MOB_ANI_SKILL1,
	MOB_ANI_CAST2,
	MOB_ANI_SKILL2,
	MOB_ANI_ETC,

	MAX_MOB_ANI
};

enum AvtMotionType {
	AVT_ANI_STOP1	= 0,
	AVT_ANI_STOP2	= 1,
	AVT_ANI_WALK	= 2,
	AVT_ANI_RUN		= 3,
	AVT_ANI_SITTING	= 4,
	AVT_ANI_SIT		= 5,
	AVT_ANI_STANDUP = 6,
	AVT_ANI_STOP3	= 7,
	AVT_ANI_ATTACK	= 8,	
	AVT_ANI_ATTACK2	= 9,
	AVT_ANI_ATTACK3	= 10,

	AVT_ANI_HIT		= 11,
	AVT_ANI_FALL	= 12,
	AVT_ANI_DIE		= 13,
	AVT_ANI_RAISE	= 14,
	AVT_ANI_JUMP1	= 15,
	AVT_ANI_JUMP2	= 16,

	AVT_ANI_PICKITEM = 17,
	MAX_AVT_ANI
};

static const char* AvatarAnimName[] = {
	"Stop1",
	"Stop2",
	"Walk",
	"Run",
	"Sitting",
	"Sit",
	"Standup",
	"Stop3",
	"Attack1",
	"Attack2",
	"Attack3",
	"Hit",
	"Fall",
	"Die",
	"Raise",
	"Jump1",
	"Jump2",
	"PickItem",
	0
};

#endif
