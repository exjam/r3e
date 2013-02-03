#include "Global.h"

#include "NetworkManager.hpp"

void NetworkManager::RegisterPacketHandlers(){
	memset(mPacketHandlers, 0, sizeof(PacketHandler) * 0x100);

	REGISTER_PACKET_HANDLER(0x704, pakChannelList);
	REGISTER_PACKET_HANDLER(0x708, pakLoginResult);
	REGISTER_PACKET_HANDLER(0x70A, pakServerSelect);
	REGISTER_PACKET_HANDLER(0x70C, pakServerJoin);
	REGISTER_PACKET_HANDLER(0x711, pakGameServerIP);
	REGISTER_PACKET_HANDLER(0x712, pakCharacterList);
	REGISTER_PACKET_HANDLER(0x715, pakPlayerData);
	REGISTER_PACKET_HANDLER(0x753, pakJoinZone);
	REGISTER_PACKET_HANDLER(0x783, pakLocalChat);
	REGISTER_PACKET_HANDLER(0x791, pakSpawnNpc);
	REGISTER_PACKET_HANDLER(0x792, pakSpawnMonster);
	REGISTER_PACKET_HANDLER(0x793, pakSpawnAvatar);
	REGISTER_PACKET_HANDLER(0x794, pakDeleteObject);
	REGISTER_PACKET_HANDLER(0x797, pakMoveNpc);
	REGISTER_PACKET_HANDLER(0x799, pakDamage);
	REGISTER_PACKET_HANDLER(0x79A, pakMovePlayer);
	REGISTER_PACKET_HANDLER(0x7A8, pakTeleport);
	REGISTER_PACKET_HANDLER(0x7FF, pakNetworkStatus);
}

DEFINE_PACKET_HANDLER(pakNetworkStatus){
	int status = pak->Read<char>();
	unsigned int key = pak->Read<unsigned int>();

	PakNetStatusEvent* evt = CreateEvent<PakNetStatusEvent>(srv);
	evt->mStatus = status;
	evt->mKey = key;
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakChannelList){
	PakChannelListEvent* evt = CreateEvent<PakChannelListEvent>(srv);
	evt->mServer = pak->Read<int>();
	int channels = pak->Read<char>();
	for(int i = 0; i < channels; ++i){
		GameChannel* chan = &evt->mChannels.Add();
		chan->mID = pak->Read<char>();
		chan->mLowPercent = pak->Read<char>();
		chan->mHighPercent = pak->Read<char>();
		chan->mUserPercent = pak->Read<short>();
		chan->mName = pak->ReadString();
	}
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakLoginResult){
	PakLoginResultEvent* evt = CreateEvent<PakLoginResultEvent>(srv);
	
	evt->mResult = pak->Read<char>();
	evt->mRight = pak->Read<short>();
	evt->mPayType = pak->Read<short>();

	if(evt->mResult == 0){
		while(!pak->IsEOF()){
			WorldServer* srv = &evt->mServers.Add();
			srv->mName = pak->ReadString();
			srv->mID = pak->Read<int>();
		}
	}

	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakServerSelect){
	PakSelectServerEvent* evt = CreateEvent<PakSelectServerEvent>(srv);
	evt->mResult = pak->Read<char>();
	evt->mUserID = pak->Read<int>();
	evt->mCryptSeed = pak->Read<int>();
	evt->mIP = pak->ReadString();
	evt->mPort = pak->Read<short>();
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakServerJoin){
	PakServerJoinEvent* evt = CreateEvent<PakServerJoinEvent>(srv);
	evt->mResult = pak->Read<char>();
	evt->mKey = pak->Read<int>();
	evt->mPayFlag = pak->Read<int>();
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakCharacterList){
	PakCharacterListEvent* evt = CreateEvent<PakCharacterListEvent>(srv);

	unsigned int characters = pak->Read<char>();
	for(unsigned int i = 0; i < characters; ++i){
		PakCharacterListEvent::Character* chr = &evt->mCharacters.Add();
		chr->mName = pak->ReadString();
		chr->mRace = pak->Read<char>();
		chr->mLevel = pak->Read<short>();
		chr->mClass = pak->Read<short>();
		chr->mDeleteTime = pak->Read<unsigned int>();
		chr->mIsPlatinum = pak->Read<char>() != 0;
		pak->ReadBytes(chr->mItems, sizeof(tagPartItem) * MAX_BODY_PART);
	}

	Emit(evt);

	return true;
}

DEFINE_PACKET_HANDLER(pakGameServerIP){
	PakGameServerIPEvent* evt = CreateEvent<PakGameServerIPEvent>(srv);
	evt->mPort = pak->Read<short>();
	evt->mUserID = pak->Read<unsigned int>();
	evt->mCryptSeed = pak->Read<unsigned int>();
	evt->mIP = pak->ReadString();
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakPlayerData){
	PakPlayerDataEvent* evt = CreateEvent<PakPlayerDataEvent>(srv);
	pak->ReadPtr(evt->mData);
	evt->mName = pak->ReadString();
	evt->mData->mPosition /= 100.0f;
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakJoinZone){
	//===========MUCH MORE DATA IN THIS PACKET========
	PakNetJoinZone* evt = CreateEvent<PakNetJoinZone>(srv);
	evt->mClientID = pak->Read<unsigned short>();
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakMovePlayer){
	PakNetMovePlayerEvent* evt = CreateEvent<PakNetMovePlayerEvent>(srv);
	evt->mClientID = pak->Read<unsigned short>();
	evt->mTargetID = pak->Read<unsigned short>();
	evt->mDistance = pak->Read<unsigned short>();
	evt->mTargetPosition = pak->Read<Vector2>() / 100.0f;
	evt->z = pak->Read<short>();
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakMoveNpc){
	PakNetMoveNpcEvent* evt = CreateEvent<PakNetMoveNpcEvent>(srv);
	evt->mClientID = pak->Read<unsigned short>();
	evt->mTargetID = pak->Read<unsigned short>();
	evt->mDistance = pak->Read<unsigned short>();
	evt->mTargetPosition = pak->Read<Vector2>() / 100.0f;
	evt->z = pak->Read<short>();
	evt->mMoveMode = pak->Read<unsigned char>();
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakSpawnNpc){
	PakSpawnNpcEvent* evt = CreateEvent<PakSpawnNpcEvent>(srv);
	pak->ReadPtr(&evt->mData);
	evt->mData.mPosition /= 100.0f;
	evt->mData.mTargetPosition /= 100.0f;
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakSpawnMonster){
	PakSpawnMonsterEvent* evt = CreateEvent<PakSpawnMonsterEvent>(srv);
	pak->ReadPtr(&evt->mData);
	evt->mData.mPosition /= 100.0f;
	evt->mData.mTargetPosition /= 100.0f;
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakSpawnAvatar){
	PakSpawnAvatarEvent* evt = CreateEvent<PakSpawnAvatarEvent>(srv);
	pak->ReadPtr(&evt->mData);
	evt->mName = pak->ReadString();

	evt->mData.mPosition /= 100.0f;
	evt->mData.mTargetPosition /= 100.0f;

	if(evt->mData.mSubFlag == PSF_SHOP){
		evt->mShop.mType = pak->Read<short>();
		evt->mShop.mTitle = pak->ReadString();
	}
	
	if(!pak->IsEOF()){
		evt->mClanData.mID = pak->Read<unsigned long>();
		evt->mClanData.mMarkBack = pak->Read<unsigned short>();
		evt->mClanData.mMarkFront = pak->Read<unsigned short>();
		evt->mClanData.mLevel = pak->Read<unsigned char>();
		evt->mClanData.mRank = pak->Read<unsigned char>();
		evt->mClanData.mName = pak->ReadString();
	}

	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakDeleteObject){
	PakDeleteObjectEvent* evt = CreateEvent<PakDeleteObjectEvent>(srv);

	evt->mCount = (pak->mSize - 6) / 2;
	for(unsigned int i = 0; i < evt->mCount; ++i)
		evt->mClientIDs[i] = pak->Read<unsigned short>();

	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakTeleport){
	PakNetTeleportEvent* evt = CreateEvent<PakNetTeleportEvent>(srv);
	evt->mClientID = pak->Read<unsigned short>();
	evt->mZone = pak->Read<unsigned short>();
	evt->mPosition = pak->Read<Vector2>() / 100.0f;
	evt->mRunMode = pak->Read<char>();
	evt->mRideMode = pak->Read<char>();
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakLocalChat){
	PakNetLocalChat* evt = CreateEvent<PakNetLocalChat>(srv);
	evt->mClientID = pak->Read<unsigned short>();
	evt->mMessage = pak->ReadString();
	Emit(evt);
	return true;
}

DEFINE_PACKET_HANDLER(pakDamage){
	pakDamageData dmg;
	pak->ReadPtr(&dmg);

	PakNetDamage* evt = CreateEvent<PakNetDamage>(srv);
	evt->mAttackerCID = dmg.mAttackerCID;
	evt->mDefenderCID = dmg.mDefenderCID;
	evt->mValue = dmg.mValue;
	evt->mAction = dmg.mAction;
	Emit(evt);

	return true;
}

//0x716 - Inventory
//0x71B - Quest Information
//0x7E0 - Clan Packet
//0x7E1 - Friends List
