#ifndef NETEVENT_PACKETS_HPP
#define NETEVENT_PACKETS_HPP

#include "Array.hpp"
#include "String.hpp"
#include "RoseStructs.hpp"
#include "NetworkEvent.hpp"
#include "RoseConstants.hpp"

struct PakNetStatusEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_NET_STATUS;

	int mStatus;
	int mKey;
};

struct WorldServer {
	String mName;
	int mID;
};

struct GameChannel {
	int mID;
	int mLowPercent;
	int mHighPercent;
	int mUserPercent;
	String mName;
};

struct PakChannelListEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_CHANNEL_LIST;

	PakChannelListEvent(){}
	~PakChannelListEvent(){
		mChannels.Clear();
	}

	int mServer;
	Array<GameChannel, 5> mChannels;
};

struct PakLoginResultEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_LOGIN_RESULT;

	PakLoginResultEvent(){}
	~PakLoginResultEvent(){
		mServers.Clear();
	}
	
	int mResult;
	int mRight;
	int mPayType;

	Array<WorldServer, 2> mServers;
};

struct PakSelectServerEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_SELECT_SERVER;

	PakSelectServerEvent(){}
	~PakSelectServerEvent(){}

	int mResult;
	int mUserID;
	int mCryptSeed;
	String mIP;
	int mPort;
};

struct PakServerJoinEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_SERVER_JOIN;

	int mResult;
	int mKey;
	int mPayFlag;
};

struct PakCharacterListEvent : public NetworkEvent {
	struct Character {
		String mName;
		int mRace;
		int mLevel;
		int mClass;
		unsigned int mDeleteTime;
		bool mIsPlatinum;

		tagPartItem mItems[MAX_BODY_PART];
	};

	static const unsigned int EventType = NET_PAK_CHARACTER_LIST;

	PakCharacterListEvent(){}
	~PakCharacterListEvent(){
		mCharacters.Clear();
	}

	Array<Character, 5> mCharacters;
};

struct PakGameServerIPEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_GAME_SERVER_IP;

	PakGameServerIPEvent(){}
	~PakGameServerIPEvent(){}

	short mPort;
	unsigned int mUserID;
	unsigned int mCryptSeed;
	String mIP;
};

struct PakPlayerDataEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_PLAYER_DATA;

	PakPlayerDataEvent(){
		mData = new pakPlayerData();
	}

	~PakPlayerDataEvent(){
		delete mData;
	}

	pakPlayerData* mData;//Not enough memory for the full thing - we don't want massive NetworkEvent pool
	String mName;
};

struct PakNetMovePlayerEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_MOVE_PLAYER;

	unsigned short mClientID;
	unsigned short mTargetID;
	unsigned short mDistance;
	Vector2 mTargetPosition;
	short z;
};

struct PakNetMoveNpcEvent : public PakNetMovePlayerEvent {
	static const unsigned int EventType = NET_PAK_MOVE_NPC;

	unsigned char mMoveMode;
};

struct PakNetJoinZone : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_JOIN_ZONE;

	unsigned int mClientID;
};

struct PakSpawnNpcEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_SPAWN_NPC;

	pakSpawnNPCData mData;
};

struct PakSpawnMonsterEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_SPAWN_MONSTER;

	pakSpawnMonsterData mData;
};

struct PakSpawnAvatarEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_SPAWN_AVATAR;

	PakSpawnAvatarEvent(){}
	~PakSpawnAvatarEvent(){}

	String mName;
	pakSpawnPlayerData mData;
	tagShopID mShop;
	tagClanID mClanData;
};

struct PakDeleteObjectEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_DELETE_OBJECT;

	unsigned int mCount;
	unsigned short mClientIDs[0];
};

struct PakNetTeleportEvent : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_TELEPORT;

	unsigned short mClientID;
	unsigned short mZone;
	Vector2 mPosition;
	char mRunMode;
	char mRideMode;
};

struct PakNetLocalChat : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_LOCAL_CHAT;

	PakNetLocalChat(){}
	~PakNetLocalChat(){}

	unsigned short mClientID;
	String mMessage;
};

struct PakNetDamage : public NetworkEvent {
	static const unsigned int EventType = NET_PAK_DAMAGE;

	unsigned short mAttackerCID;
	unsigned short mDefenderCID;
	unsigned short mValue;
	unsigned short mAction;
};

#endif