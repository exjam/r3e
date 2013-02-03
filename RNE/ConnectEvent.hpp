#ifndef CONNECT_EVENT_HPP
#define CONNECT_EVENT_HPP

#include "NetworkEvent.hpp"

struct ConnectEvent : public NetworkEvent {
	static const unsigned int EventType = NET_CONNECT_RESULT;

	bool mSuccess;
};


struct DisconnectEvent : public NetworkEvent {
	static const unsigned int EventType = NET_DISCONNECT;
};

#endif