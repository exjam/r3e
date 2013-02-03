#ifndef TITAN_FILE_H
#define TITAN_FILE_H

#include "File.hpp"
#include "SafeDelete.hpp"

class TitanFile : public File {
public:
	TitanFile() : mBuffer(0), mPosition(0), mSize(0) {}
	TitanFile(const char* path, const char* method)
		: mBuffer(0), mPosition(0), mSize(0)
	{
		Open(path, method);
	}

	~TitanFile(){
		Close();
	}

	void SetData(unsigned char* buffer, long size){
		mPosition = 0;
		mSize = size;
		mBuffer = buffer;
	}

	bool Open(const char* /*path*/, const char* /*method*/){
		return true;
	}

	bool IsOpen(){
		return (mBuffer != NULL);
	}

	void Close(){
		SAFE_DELETE_ARRAY(mBuffer);
	}

	int ReadData(void* data, int size){
		if(mPosition + size > mSize) size = mSize - mPosition;
		memcpy_s(data, size, mBuffer + mPosition, size);
		mPosition += size;
		return size;
	}

	int WriteData(void* data, int size){
		memcpy_s(mBuffer + mPosition, mSize - mPosition, data, size);
		return size;
	}

	int WriteData(void* data, int size, int pos){
		memcpy_s(mBuffer + pos, mSize - pos, data, size);
		return size;
	}

	void Skip(int bytes){
		mPosition += bytes;
	}

	void Seek(int position){
		mPosition = position;
	}

	long Position(){
		return mPosition;
	}

	long Size(){
		return mSize;
	}

	bool IsEOF(){
		return mPosition >= mSize;
	}

private:
	long mSize;
	long mPosition;
	unsigned char* mBuffer;
};

#endif