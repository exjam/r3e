#include "Global.h"

#include "Connection.hpp"

Connection::Connection(int id, const char* ip, int port, int cryptKey){
	mID = id;
	mAddr.sin_family = AF_INET;
	mAddr.sin_addr.s_addr = inet_addr(ip);
	mAddr.sin_port = htons(u_short(port));
	mState = STATE_WAITING;
	mSocket = 0;
	mRecvSize = 6;
	mRecvCur = 0;
	mSendSize = 0;

	mKey = 0;

	mDisconnect = false;

	if(cryptKey == 0)
		mCrypt.Default();
	else
		mCrypt.Init(cryptKey);
}

Connection::~Connection(){
	if(mSocket){
		shutdown(mSocket, SD_BOTH);
		closesocket(mSocket);
		mSocket = 0;
	}
}

bool Connection::Connect(){
	if(mSocket == 0)
		mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	mState = STATE_ERROR;
	if(connect(mSocket, (SOCKADDR*)&mAddr, sizeof(mAddr)) == SOCKET_ERROR)
		return false;

	mState = STATE_CONNECTED;
	return true;
}

void Connection::Disconnect(){
	mState = STATE_DISCONNECTING;
}

bool Connection::IsWaiting(){
	return mState == STATE_WAITING;
}

bool Connection::IsDisconnecting(){
	return mState == STATE_DISCONNECTING;
}

bool Connection::IsConnected(){
	return mState == STATE_CONNECTED;
}

bool Connection::SendQueueFull(){
	return mSendQueueFull;
}

void Connection::QueuePacket(Packet* pak){
	if(mSendSize + pak->mSize > 0x7FF){
		mSendQueueFull = true;	
		return;
	}

	mSendQueueFull = false;

	printf("Send ");
	for(unsigned int i = 0; i < pak->mSize; ++i)
		printf("%02x ", pak->mBuffer[i]);
	printf("\n");

	int size = pak->mSize;
	pak->mBuffer[4] = 0;
	pak->mBuffer[5] = 0;
	mCrypt.ESCP(pak->mBuffer, mKey);

	mSendSection.Enter();
	memcpy(mSendBuffer + mSendSize, pak, size);
	mSendSize += size;
	mSendSection.Leave();
}

bool Connection::DecryptHeader(){
	mRecvSize = mCrypt.DRSH(mRecvPacket.mBuffer);
	return mRecvSize >= 6;
}

bool Connection::DecryptBody(){
	mRecvPacket.mReadPos = 6;
	return mCrypt.DRSB(mRecvPacket.mBuffer);
}
