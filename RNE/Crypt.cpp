#include "Global.h"

#include "Crypt.hpp"

template<typename T, typename U>
inline void FlipHeadMain(T a, U b){
	a->AddBufferLen1 = b->AddBufferLen1;
	a->AddBufferLen2 = b->AddBufferLen2;
	a->AddBufferLen3 = b->AddBufferLen3;
	a->AddBufferLen4 = b->AddBufferLen4;
	a->Command1 = b->Command1;
	a->Command2 = b->Command2;
	a->Command3 = b->Command3;
	a->Command4 = b->Command4;
	a->EncryptValue1 = b->EncryptValue1;
	a->EncryptAddValue1 = b->EncryptAddValue1;
	a->EncryptAddValue2 = b->EncryptAddValue2;
}

template<typename T, typename U>
inline void FlipHeadFinal(T a, U b){
	a->AddTableValue1 = b->AddTableValue1;
	a->AddTableValue2 = b->AddTableValue2;
	a->AddTableValue3 = b->AddTableValue3;
	a->AddTableValue4 = b->AddTableValue4;
}

Crypt::Crypt(){
	mCryptTable = new unsigned int*[0x10];

	for(unsigned int i = 0; i < 0x10; ++i)
		mCryptTable[i] = new unsigned int[0x800];

	mCryptIndex = new unsigned short[0x200];
}

Crypt::~Crypt(){
	for(unsigned int i = 0; i < 0x10; ++i)
		delete[] mCryptTable[i];

	delete[] mCryptTable;
	delete[] mCryptIndex;
}

void Crypt::ESSP(unsigned char* buffer){
	Head head = { 0 };
	head.AddTableValue = rand() % 0x1FF + 1;
	head.EncryptAddValue = rand() % 0xF + 1;
	head.EncryptValue = head.AddTableValue + head.EncryptAddValue;
	head.AddBufferLen = *((unsigned short*)&buffer[0]);
	head.Command = *((unsigned short*)&buffer[2]);
	FlipHeadMain((HeadCryptedServer*)buffer, (HeadDecrypted*)&head);

	unsigned char csum = 0;

	for(unsigned int i = 0; i < 5; i++){
		csum = gCrcTable[((unsigned char*)&head)[i] ^ csum];
		buffer[i] ^= mCryptTable[i][head.AddTableValue];
	}

	for(unsigned int i = 6; i < head.AddBufferLen; i++){
		csum = gCrcTable[buffer[i] ^ csum];
		buffer[i] ^= mCryptTable[(head.EncryptAddValue + i) & 0xF][(head.AddTableValue + i) & 0x7FF];
	}

	buffer[5] = csum;

	FlipHeadFinal((HeadCryptedServer*)buffer, (HeadDecrypted*)&head);
}

unsigned short Crypt::DRSH(unsigned char* buffer){
	Head head = { 0 };
	FlipHeadFinal((HeadDecrypted*)&head, (HeadCryptedServer*)buffer);

	for(unsigned int i = 0; i < 5; i++){
		buffer[i] ^= mCryptTable[i][head.AddTableValue];
	}

	FlipHeadMain((HeadDecrypted*)&head, (HeadCryptedServer*)buffer);
	memcpy(buffer, &head, 5);
	return head.AddBufferLen;
}

bool Crypt::DRSB(unsigned char* buffer){
	Head head = { 0 };
	memcpy(&head, buffer, 6);

	unsigned char csum = 0;
	for(unsigned int i = 0; i < 5; i++)
		csum = gCrcTable[((unsigned char*)&head)[i] ^ csum];
	
	for(unsigned int i = 6; i < head.AddBufferLen; ++i){
		buffer[i] ^= mCryptTable[(head.EncryptAddValue + i) & 0xF][(head.AddTableValue + i) & 0x7FF];
		csum = gCrcTable[buffer[i] ^ csum];
	}

	if(csum != buffer[5])
		return false;

	*((unsigned short*)&buffer[0]) = head.AddBufferLen;
	*((unsigned short*)&buffer[2]) = head.Command;

	return true;
}

int Crypt::ESCP(unsigned char* buffer, int& key){
	unsigned int RealPakSize = *((unsigned short*)&buffer[0]);

	Head head = { 0 };
	head.EncryptValue = 0;//rand();
	head.AddTableValue = mCryptIndex[key & 0x1FF];
	head.EncryptAddValue = ++key + head.EncryptValue;
	head.AddBufferLen = RealPakSize + head.EncryptValue;
	head.Command = *((unsigned short*)&buffer[2]);
	FlipHeadMain((HeadCryptedClient*)buffer, (HeadDecrypted*)&head);

	unsigned char csum = 0;
	for(unsigned int i = 0; i < 5; ++i){
		csum = gCrcTable[((unsigned char*)&head)[i] ^ csum];
		buffer[i] ^= mCryptTable[i][head.AddTableValue];
	}

	for(unsigned int i = 6; i < RealPakSize; i++){
		csum = gCrcTable[buffer[i] ^ csum];
		buffer[i] ^= mCryptTable[(head.EncryptAddValue + i) & 0xF][(head.AddTableValue + i) & 0x7FF];
	}

	buffer[5] = csum;

	FlipHeadFinal((HeadCryptedClient*)buffer, (HeadDecrypted*)&head);

	return head.AddBufferLen;
}

unsigned short Crypt::DRCH(unsigned char* buffer){
	Head head = { 0 };
	FlipHeadFinal((HeadDecrypted*)&head, (HeadCryptedClient*)buffer);

	for(int i = 0; i < 5; ++i)
		buffer[i] ^= mCryptTable[i][head.AddTableValue];

	FlipHeadMain((HeadDecrypted*)&head, (HeadCryptedClient*)buffer);
	memcpy(buffer, &head, 5);
	return head.AddBufferLen;
}

bool Crypt::DRCB(unsigned char* buffer){
	Head head = { 0 };
	memcpy(&head, buffer, 6);
	unsigned short buflen = (unsigned short)(head.AddBufferLen - head.EncryptValue);

	unsigned char csum = 0;
	for(unsigned int i = 0; i < 5; ++i)
		csum = gCrcTable[((unsigned char*)&head)[i] ^ csum];

	for(unsigned int i = 6; i < buflen; ++i){
		buffer[i] ^= mCryptTable[(head.EncryptAddValue + i) & 0xF][(head.AddTableValue + i) & 0x7FF];
		csum = gCrcTable[buffer[i] ^ csum];
	}

	if(csum != buffer[5])
		return false;

	*((unsigned short*)&buffer[0]) = buflen;
	*((unsigned short*)&buffer[2]) = head.Command;

	return true;
}

bool Crypt::Find_DupTABLE(unsigned int val, unsigned int table, unsigned int range){
	for(unsigned int i = 0; i < table; i++){
		for(int j = 0; j < 0x7FF; j++){
			if(mCryptTable[i][j] == val)
				return true;
		}
	}

	for(unsigned int i = 0; i < range; i++){
		if(mCryptTable[table][i] == val)
			return true;
	}

	return false;
}

bool Crypt::Find_DupINDEX(unsigned int val, unsigned int range){
	for(unsigned int i = 0; i < range; i++){
		if(mCryptIndex[i] == val)
			return true;
	}

	return false;
}

void Crypt::Default(){
	CR001 seed;
	seed.Init(0x0042D266);
	for(unsigned int i = 0; i < 0x10; i++){
		seed.SetType(i & 3);
		for(unsigned int j = 0; j < 0x800; j++){
			do {
				mCryptTable[i][j] = seed.Get();
			} while(Find_DupTABLE(mCryptTable[i][j], 0, j));
		}
	}

	for(unsigned int i = 0; i < 0x200; i++){
		do {
			mCryptIndex[i] = seed.R_AC() & 0x7FF;
		} while(Find_DupINDEX(mCryptIndex[i], i));
	}
}

void Crypt::Init(unsigned int key){
	unsigned char TableFuncList[0x11];
	CR001 seed;

	seed.Init(key);
	key = seed.R_MY();
	seed.Init(key);

	for(unsigned int i = 0; i < 0x11; i++)
		TableFuncList[i] = (unsigned char)seed.R_BC();

	for(unsigned int i = 0; i < 0x10; i++){
		seed.SetType(TableFuncList[i] & 3);
		for(unsigned int j = 0; j < 0x800; j++){
			do {
				mCryptTable[i][j] = seed.Get();
			} while(Find_DupTABLE(mCryptTable[i][j], 0, j));
		}
	}

	seed.SetType(TableFuncList[0x10] & 3);
	for(unsigned int i = 0; i < 0x200; i++){
		do {
			mCryptIndex[i] = seed.Get() & 0x7FF;
		} while(Find_DupINDEX(mCryptIndex[i], i));
	}
}
