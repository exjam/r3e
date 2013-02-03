#ifndef CLIENT_OBJECT_HPP
#define CLIENT_OBJECT_HPP

enum ClientObjectType {
	CO_AVATAR,
	CO_NPC,
	CO_MONSTER,
	CO_PLAYER,
};

class ClientObject {
public:
	ClientObject(){}
	virtual ~ClientObject(){}

	void Update(){
	}

	unsigned short ClientID(){
		return mClientID;
	}

	void SetClientID(unsigned short id){
		mClientID = id;
	}

	short ObjectType(){
		return mObjectType;
	}

protected:
	unsigned short mClientID;
	unsigned short mObjectType;
};

#endif
