#ifndef PACKET_HPP
#define PACKET_HPP

#include "String.hpp"

class Packet {
public:
	Packet() : mSize(6), mCommand(0) {}
	Packet(unsigned short cmd) : mSize(6), mCommand(cmd) {}
	~Packet(){}

	template<class T> void Add(T& value){
		*((T*)(mBuffer + mSize)) = value;

		mSize += sizeof(T);
	}

	template<> void Add(const String& value){
		unsigned short size = unsigned short(value.Length());
		memcpy(mBuffer + mSize, value.Str(), size);
		mSize += size;
		mBuffer[++mSize] = 0;
	}

	template<> void Add(String& value){
		unsigned short size = unsigned short(value.Length());
		memcpy(mBuffer + mSize, value.Str(), size);
		mSize += size;
		mBuffer[mSize++] = 0;
	}

	template<> void Add(const char*& value){
		unsigned short size = unsigned short(strlen(value));
		memcpy(mBuffer + mSize, value, size);
		mSize += size;
		mBuffer[++mSize] = 0;
	}

	template<> void Add(char*& value){
		unsigned short size = unsigned short(strlen(value));
		memcpy(mBuffer + mSize, value, size);
		mSize += size;
		mBuffer[++mSize] = 0;
	}

	template<class T> void AddVal(const T value){
		*((T*)(mBuffer + mSize)) = value;

		mSize += sizeof(T);
	}

	template<class T> void AddPtr(T* value){
		*((T*)(mBuffer + mSize)) = *value;

		mSize += sizeof(T);
	}

	void Add(const char* value, unsigned short size){
		memcpy(mBuffer + mSize, value, size);
		mSize += size;
	}

	void Add(const unsigned char* value, unsigned short size){
		memcpy(mBuffer + mSize, value, size);
		mSize += size;
	}

	template<class T> T Read(){
		T tmp = *((T*)(mBuffer + mReadPos));
		mReadPos += sizeof(T);
		return tmp;
	}

	template<class T> void ReadPtr(T* val){
		memcpy(val, mBuffer + mReadPos, sizeof(T));
		mReadPos += sizeof(T);
	}

	void ReadBytes(void* ptr, unsigned short size){
		memcpy(ptr, mBuffer + mReadPos, size);
		mReadPos += size;
	}

	const char* ReadString(){
		const char* str = (const char*)(mBuffer + mReadPos);
		mReadPos += unsigned short(strlen(str) + 1);
		return str;
	}

	bool IsEOF(){
		return mReadPos >= mSize;
	}

	void SetSize(unsigned short size){
		mSize = size;
	}

	void SetCommand(unsigned short command){
		mCommand = command;
	}

public:
	union {
		struct {
			unsigned short mSize;
			unsigned short mCommand;
		};

		unsigned char mBuffer[0x7FF];
	};

	unsigned short mReadPos;
};

#endif
