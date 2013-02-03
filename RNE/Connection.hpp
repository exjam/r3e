#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <winsock2.h>
#include "Crypt.hpp"
#include "Packet.hpp"
#include "CriticalSection.hpp"

enum ConnectionState {
	STATE_INVALID,
	STATE_WAITING,
	STATE_CONNECTED,
	STATE_ERROR,
	STATE_DISCONNECTING,
};

class Connection {
public:
	Connection(int id, const char* ip, int port, int cryptKey = 0);
	~Connection();

	bool Connect();
	void Disconnect();

	bool IsWaiting();
	bool IsDisconnecting();
	bool IsConnected();

	bool DecryptHeader();
	bool DecryptBody();

	void QueuePacket(Packet* pak);

	bool SendQueueFull();

public:
	SOCKET mSocket;

	int mRecvSize;
	int mRecvCur;
	Packet mRecvPacket;

	CriticalSection mSendSection;
	int mSendSize;
	bool mSendQueueFull;
	unsigned char mSendBuffer[0x7FF];

	Crypt mCrypt;
	int mKey;

	int mState;
	int mID;

	sockaddr_in mAddr;

	bool mDisconnect;
};

#endif