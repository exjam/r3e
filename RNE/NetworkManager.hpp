#ifndef NETWORK_MAN_HPP
#define NETWORK_MAN_HPP

#include <winsock2.h>
#include "Array.hpp"
#include "Connection.hpp"
#include "EventQueue.hpp"
#include "NetworkEvent.hpp"
#include "ConnectEvent.hpp"
#include "PacketEvents.hpp"
#include "CriticalSection.hpp"

#define DEFINE_PACKET_HANDLER(x) bool NetworkManager::x(int srv, Packet* pak)
#define REGISTER_PACKET_HANDLER(x, y) mPacketHandlers[x - 0x700] = &NetworkManager::y;

#ifdef _DEBUG
#undef new
#endif

class NetworkManager {
private:
	typedef bool (NetworkManager::*PacketHandler)(int srv, Packet* pak);

public:
	NetworkManager();
	~NetworkManager();

	static NetworkManager& Instance(){
		return mSelf;
	}

	int Run();
	void Stop();

	bool Connect(int id, const char* ip, int port, int cryptKey);
	void Disconnect(int id);

	bool SendQueueFull(int id){
		Connection* con = GetConnection(id);
		if(!con) return true;
		return con->SendQueueFull();
	}

	bool SendPacket(int id, Packet* pak);
	void SetCryptKey(int srv, int key);

	bool StartPoll(){
		return mEvents.StartPoll();
	}

	NetworkEvent* Poll(){
		return mEvents.Poll();
	}

	void EndPoll(){
		mEvents.EndPoll();
	}

private:
	void Emit(NetworkEvent* evt){
		mEvents.Enqueue((NetworkEventData*)evt);
	}

private:
	DEFINE_PACKET_HANDLER(pakNetworkStatus);
	DEFINE_PACKET_HANDLER(pakChannelList);
	DEFINE_PACKET_HANDLER(pakLoginResult);
	DEFINE_PACKET_HANDLER(pakServerSelect);
	DEFINE_PACKET_HANDLER(pakServerJoin);
	DEFINE_PACKET_HANDLER(pakGameServerIP);
	DEFINE_PACKET_HANDLER(pakCharacterList);
	DEFINE_PACKET_HANDLER(pakPlayerData);
	DEFINE_PACKET_HANDLER(pakMovePlayer);
	DEFINE_PACKET_HANDLER(pakMoveNpc);
	DEFINE_PACKET_HANDLER(pakLocalChat);
	DEFINE_PACKET_HANDLER(pakJoinZone);
	DEFINE_PACKET_HANDLER(pakSpawnMonster);
	DEFINE_PACKET_HANDLER(pakSpawnNpc);
	DEFINE_PACKET_HANDLER(pakSpawnAvatar);
	DEFINE_PACKET_HANDLER(pakDeleteObject);
	DEFINE_PACKET_HANDLER(pakTeleport);
	DEFINE_PACKET_HANDLER(pakDamage);
	
private:
	template<class T> T* CreateEvent(int srv){
		NetworkEventData* evt = mEvents.CreateEvent();
		T* val = ::new (evt) T();
		val->_evt_type = T::EventType;
		val->srvID = srv;
		return val;
	}

	void RegisterPacketHandlers();

	void OnConnect(int id, bool success);
	void OnDisconnect(int id);
	void OnReceivePacket(int id, Packet* pak);

	Connection* GetConnection(int id);

private:
	PacketHandler mPacketHandlers[0x100];
	Array<Connection*, 10> mConnections;
	EventQueue<NetworkEventData> mEvents;
	volatile bool mActive;
	static NetworkManager mSelf;
};

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#endif