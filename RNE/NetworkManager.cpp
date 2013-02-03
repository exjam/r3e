#include "Global.h"

#include "NetworkManager.hpp"

NetworkManager NetworkManager::mSelf;

NetworkManager::NetworkManager(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	RegisterPacketHandlers();
}

NetworkManager::~NetworkManager(){
	WSACleanup();
}

Connection* NetworkManager::GetConnection(int id){
	for(unsigned int i = 0; i < mConnections.Size(); ++i){
		if(mConnections[i]->mID != id) continue;
		return mConnections[i];
	}

	return NULL;
}

bool NetworkManager::Connect(int id, const char* ip, int port, int cryptKey){
	if(GetConnection(id)) return false;
	mConnections.PushBack(new Connection(id, ip, port, cryptKey));
	return true;
}

void NetworkManager::Disconnect(int id){
	Connection* con = GetConnection(id);
	if(!con) return;

	con->Disconnect();
}

void NetworkManager::OnConnect(int id, bool success){
	ConnectEvent* evt = CreateEvent<ConnectEvent>(id);
	evt->mSuccess = success;
	Emit(evt);
}

void NetworkManager::OnDisconnect(int id){
	DisconnectEvent* evt = CreateEvent<DisconnectEvent>(id);
	Emit(evt);
}

bool NetworkManager::SendPacket(int id, Packet* pak){
	Connection* con = GetConnection(id);
	if(!con) return false;

	con->QueuePacket(pak);
	return true;
}

void NetworkManager::SetCryptKey(int srv, int key){
	Connection* con = GetConnection(srv);
	if(!con) return;
	con->mKey = key;
}

void NetworkManager::Stop(){
	mActive = false;
}

int NetworkManager::Run(){
	mActive = true;
	fd_set recvSet;
	fd_set sendSet;
	TIMEVAL tv;
	tv.tv_sec = 0;
	tv.tv_usec = 5000;

	while(mActive){
		FD_ZERO(&recvSet);
		FD_ZERO(&sendSet);

		for(unsigned int i = 0; i < mConnections.Size(); ++i){
			Connection* con = mConnections[i];
			if(con->IsWaiting()){
				if(con->Connect()){
					OnConnect(con->mID, true);
				}else{
					OnConnect(con->mID, false);
					delete con;
					mConnections.Erase(i--);
				}
			}else if(con->IsDisconnecting()){
				delete con;
				mConnections.Erase(i--);
			}else{
				FD_SET(con->mSocket, &recvSet);

				if(con->mSendSize > 0){
					FD_SET(con->mSocket, &sendSet);
				}
			}
		}
		
		if(select(0, &recvSet, &sendSet, 0, &tv) <= 0) continue;

		for(unsigned int i = 0; i < mConnections.Size(); ++i){
			Connection* con = mConnections[i];
			if(FD_ISSET(con->mSocket, &recvSet)){
				int read = recv(con->mSocket, (char*)con->mRecvPacket.mBuffer + con->mRecvCur, con->mRecvSize - con->mRecvCur, 0);
				if(read == 0){
					OnDisconnect(con->mID);
					delete con;
					mConnections.Erase(i--);
					continue;
				}else if(read != SOCKET_ERROR){
					con->mRecvCur += read;
					if(con->mRecvSize == 6){
						if(!con->DecryptHeader()){
							printf("Invalid packet header!\n");
							con->mRecvCur = 0;
							con->mRecvSize = 6;
						}
					}

					if(con->mRecvSize == 6 || con->mRecvCur == con->mRecvSize){
						if(!con->DecryptBody()){
							printf("Invalid packet body!\n");
						}else{
							OnReceivePacket(con->mID, &con->mRecvPacket);
							con->mRecvCur = 0;
							con->mRecvSize = 6;
						}
					}
				}
			}

			if(FD_ISSET(con->mSocket, &sendSet)){
				con->mSendSection.Enter();
				int sent = send(con->mSocket, (const char*)con->mSendBuffer, con->mSendSize, 0);
				con->mSendQueueFull = false;
				if(sent == SOCKET_ERROR){
					printf("Error sending!\n");
				}

				con->mSendSize -= sent;
				if(con->mSendSize != 0)
					memcpy(con->mSendBuffer, con->mSendBuffer + sent, con->mSendSize);
				
				con->mSendSection.Leave();
			}
		}
	}

	mConnections.DeleteValues();

	return 0;
}


void NetworkManager::OnReceivePacket(int id, Packet* pak){
	printf("Recv ");
	for(unsigned int i = 0; i < pak->mSize; ++i)
		printf("%02x ", pak->mBuffer[i]);
	printf("\n");

	PacketHandler func;
	if(pak->mCommand < 0x700 || pak->mCommand > 0x7FF || (func = mPacketHandlers[pak->mCommand - 0x700]) == 0){
		printf("Unhandled Packet %03X [%u bytes]\n", pak->mCommand, pak->mSize);
	}else{
		(this->*func)(id, pak);
	}
}
