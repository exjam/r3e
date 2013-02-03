#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "StateManager.hpp"
#include "..\RNE\NetworkManager.hpp"

enum ServerID {
	LS = 1,
	WS = 2,
	GS = 3,
};

extern class Network* gNetwork;

class Network {
public:
	Network(){
		mNetMan = &NetworkManager::Instance();
	}

	~Network(){}

	void Process(){
		if(!mNetMan->StartPoll()) return;

		while(NetworkEvent* evt = mNetMan->Poll())
			gActiveState->HandleEvent(evt);

		mNetMan->EndPoll();
	}

	DWORD Run(){
		mNetThread = GetCurrentThread();
		return mNetMan->Run();
	}

	void SendPacket(int srv, Packet* pak){
		mNetMan->SendPacket(srv, pak);
	}

	void SetCryptKey(int srv, int key){
		mNetMan->SetCryptKey(srv, key);
	}

	bool Connect(int id, const char* ip, int port, int cryptKey){
		return mNetMan->Connect(id, ip, port, cryptKey);
	}

	void Disconnect(int id){
		mNetMan->Disconnect(id);
	}

	void WaitEnd(){
		mNetMan->Stop();
		WaitForSingleObject(mNetThread, 500);
	}

private:
	NetworkManager* mNetMan;
	HANDLE mNetThread;
};

#endif