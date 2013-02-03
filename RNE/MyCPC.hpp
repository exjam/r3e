#ifndef CPC_HPP
#define CPC_HPP

//From TitanROSE
const unsigned char CrcTable[256] = {
    0x74, 0x69, 0x74, 0x61, 0x6E, 0x52, 0x4F, 0x53, 0x45, 0x1A, 0x59, 0xF3, 0xBF, 0xC8, 0xF9, 0x1C, 
    0x7B, 0x98, 0x30, 0x03, 0xD0, 0x98, 0xCD, 0xE5, 0x23, 0x16, 0xD2, 0x60, 0x1F, 0x3F, 0x45, 0xDF, 
    0xBF, 0xB5, 0x57, 0x47, 0x15, 0xBA, 0xAF, 0x38, 0x61, 0xED, 0xF1, 0x68, 0xC6, 0xD6, 0x03, 0x7E, 
    0x44, 0x41, 0x45, 0x20, 0x48, 0x41, 0x53, 0x20, 0x53, 0x4D, 0x41, 0x4C, 0x4C, 0x20, 0x57, 0x49, 
    0x4C, 0x4C, 0x59, 0xEF, 0xCA, 0x96, 0x07, 0x4B, 0x4E, 0xE1, 0xEA, 0x7A, 0xA0, 0xDA, 0xDB, 0xD9, 
    0x78, 0xD3, 0x70, 0x5E, 0x83, 0xC3, 0xC9, 0xAE, 0x7B, 0xC8, 0x07, 0x4B, 0xEB, 0xC7, 0x9E, 0xA7, 
    0x49, 0xC2, 0xAE, 0x36, 0x0F, 0x0C, 0xB9, 0xCB, 0x6E, 0xC6, 0x6A, 0xE1, 0xCE, 0xF5, 0xC3, 0x94, 
    0x2B, 0x7D, 0x63, 0xEB, 0x04, 0xD5, 0xB9, 0x46, 0x70, 0xE0, 0xDA, 0xF3, 0x99, 0x97, 0x4D, 0xDD, 
    0x15, 0xFB, 0xF4, 0x4A, 0xF1, 0x10, 0x0E, 0x0B, 0x6E, 0xB8, 0x47, 0x36, 0x23, 0x4D, 0x6B, 0x36, 
    0xA9, 0xBC, 0xBF, 0xC3, 0xF8, 0x01, 0xF7, 0xA2, 0x04, 0x5A, 0x5B, 0x2C, 0x56, 0x77, 0x0F, 0x10, 
    0x43, 0x8F, 0x24, 0x41, 0xD4, 0x98, 0x3C, 0xFD, 0x0F, 0x11, 0x0C, 0xF6, 0xC4, 0x87, 0xF8, 0x6F, 
    0x73, 0x45, 0x63, 0x52, 0x65, 0x54, 0x20, 0x4D, 0x65, 0x53, 0x73, 0x41, 0x67, 0x45, 0x73, 0x20, 
    0x61, 0x72, 0x65, 0x20, 0x66, 0x75, 0x6E, 0xBD, 0xA6, 0xEF, 0x79, 0x03, 0xB1, 0xD2, 0x87, 0x11, 
    0x72, 0x2D, 0x13, 0x57, 0xC8, 0x8D, 0x22, 0xE8, 0x45, 0xAA, 0x2B, 0xEE, 0x1B, 0x91, 0xC6, 0x88, 
    0xDE, 0x83, 0xF9, 0x1C, 0x4C, 0x2E, 0xCE, 0x86, 0x1E, 0x3B, 0x03, 0x22, 0xB8, 0xDE, 0x23, 0x11, 
    0xA5, 0x65, 0x34, 0x49, 0xB9, 0xED, 0x50, 0x4C, 0xC6, 0xC9, 0x61, 0x04, 0xC2, 0x2E, 0x58, 0x3B, 
};

class CPC {
public:
	CPC(){}
	~CPC(){}

	bool Find_DupINDEX(unsigned int val, unsigned int range){
		for(unsigned int i = 0; i < range; ++i){
			if(mCryptIndex[i] == val)
				return true;
		}

		return false;
	}

	bool Find_DupTABLE(unsigned int val, unsigned int table, unsigned int range){
		for(unsigned int i = 0; i < table; ++i){
			for(unsigned int j = 0; j < 0x7FF; ++j){
				if(mCryptTable[i][j] == val)
					return true;
			}
		}

		for(unsigned int i = 0; i < range; ++i){
			if(mCryptTable[table][i] == val)
				return true;
		}

		return false;
	}

	void Default(){
		Seed seed;
		seed.Init(0x0042D266);
		for(unsigned int i = 0; i < 16; ++i){
			seed.SetType(i & 3);

			for(unsigned int j = 0; j < 0x800; ++j){
				do {
					mCryptTable[i][j] = seed.Get();
				} while(Find_DupTABLE(mCryptTable[i][j], 0, j));
			}
		}

		for(unsigned int i = 0; i < 512; ++i){
			do {
				mCryptIndex[i] = seed.GetAC() & 0x7FF;
			} while(Find_DupINDEX(mCryptIndex[i], i));
		}
	}

	void Init(int key){
		unsigned char seedTypes[17];
		Seed seed;

		seed.Init(key);
		key = seed.GetMY();
		seed.Init(key);

		for(unsigned int i = 0; i < 17; ++i)
			seedTypes[i] = (unsigned char)seed.GetBC();

		for(unsigned int i = 0; i < 16; ++i){
			seed.SetType(seedTypes[i] & 3);
			for(unsigned int j = 0; j < 2048; ++j){
				do {
					mCryptTable[i][j] = seed.Get();
				} while(Find_DupTABLE(mCryptTable[i][j], 0, j));
			}
		}

		seed.SetType(seedTypes[16] & 3);
		for(unsigned int i = 0; i < 512; ++i){
			do {
				mCryptIndex[i] = seed.Get() & 0x7FF;
			} while(Find_DupINDEX(mCryptIndex[i], i));
		}
	}

	void ESCP(unsigned char* buffer, int& key){
		unsigned short* wbuffer = (unsigned short*)buffer;
		unsigned int size = wbuffer[0] & 0x7FF;
		unsigned int cmd = wbuffer[1] & 0x7FF;

		unsigned __int64 encryptValue = 0;//rand() & 0x7;
		unsigned __int64 addTableValue = mCryptIndex[key & 0x1FF] & 0x7FF;
		unsigned __int64 encryptAddValue = (++key + encryptValue) & 0xF;

		unsigned __int64 head = (size + encryptValue) & 0x7FF;
		head |= cmd << 11;
		head |= addTableValue << 22;
		head |= encryptAddValue << 33;
		head |= encryptValue << 37;

		unsigned __int64 dest = *((unsigned __int64*)buffer);
		dest &= ~0xC7F3FFF1C7;
		dest |= (head << 6) & (7 << 6);
		dest |= (head << 15) & (7 << 18);
		dest |= (head << 17) & (7 << 23);
		dest |= ((head >> 9) & 3) << 38;
		dest |= (head << 1) & (7 << 12);
		dest |= (head >> 14) & 7;
		dest |= ((head >> 17) & 7) << 32;
		dest |= (head << 8) & (3 << 28);
		dest |= ((head >> 37) & 7) << 15;
		dest |= ((head >> 33) & 3) << 30;
		dest |= (head >> 14) & (3 << 21);
		*((unsigned __int64*)buffer) = dest;

		unsigned char csum = 0;
		for(unsigned int i = 0; i < 5; ++i){
			csum = CrcTable[((unsigned char*)&head)[i] ^ csum];
			buffer[i] ^= mCryptTable[i][addTableValue];
		}

		for(unsigned int i = 6; i < size; ++i){
			csum = CrcTable[buffer[i] ^ csum];
			buffer[i] ^= mCryptTable[(encryptAddValue + i) & 0xF][(addTableValue + i) & 0x7FF];
		}

		buffer[5] = csum;

		dest = *((unsigned __int64*)buffer);
		dest &= ~0x381C000E38;
		dest |= ((head >> 22) & 7) << 35;
		dest |= (head >> 22) & (7 << 3);
		dest |= (head >> 19) & (7 << 9);
		dest |= (head >> 5) & (7 << 26);
		*((unsigned __int64*)buffer) = dest;
	}

	unsigned short DRSH(unsigned char* buffer){
		unsigned __int64 head = 0;
		unsigned __int64 src = *((unsigned __int64*)buffer);

		head |= (src >> 3) & 7;
		head |= (src >> 32) & (7 << 3);
		head |= (src >> 9) & (7 << 6);
		head |= (src >> 12) & (3 << 9);

		for(unsigned int i = 0; i < 5; ++i){
			buffer[i] ^= mCryptTable[i][head];
		}

		src = *((unsigned __int64*)buffer);

		head <<= 22;

		head |= (src >> 32) & 7;
		head |= (src << 3) & (7 << 3);
		head |= (src >> 6) & (7 << 6);
		head |= (src >> 19) & (3 << 9);

		head |= (src >> 12) & (7 << 11);
		head |= (src >> 4) & (7 << 14);
		head |= (src << 11) & (7 << 17);
		head |= (src >> 18) & (3 << 20);

		head |= ((src >> 26) & 3) << 33;
		head |= ((src >> 30) & 3) << 35;

		head |= ((src >> 9) & 7) << 37;

		memcpy(buffer, &head, 5);

		return head & 0x7FF;
	}

	bool DRSB(unsigned char* buffer){
		unsigned __int64 head = 0;
		memcpy(&head, buffer, 6);

		unsigned int addBufferLen = head & 0x7FF;
		unsigned int command = (head >> 11) & 0x7FF;
		unsigned int addTableValue = (head >> 22) & 0x7FF;
		unsigned int encryptAddValue = (head >> 33) & 0xF;

		unsigned char csum = 0;
		for(unsigned int i = 0; i < 5; ++i){
			csum = CrcTable[((unsigned char*)&head)[i] ^ csum];
		}

		for(unsigned int i = 6; i < addBufferLen; ++i){
			buffer[i] ^= mCryptTable[(encryptAddValue + i) & 0xF][(addTableValue + i) & 0x7FF];
			csum = CrcTable[buffer[i] ^ csum];
		}

		if(csum != buffer[5])
			return false;

		*((unsigned short*)&buffer[0]) = addBufferLen;
		*((unsigned short*)&buffer[2]) = command;

		return true;
	}

private:
	int mCryptTable[16][2048];
	unsigned short mCryptIndex[512];
	
	struct Seed {
		void Init(unsigned int seed){
			mVcSeed = seed;
			mBcSeed = seed;
			mAcSeed = seed;
			mMySeed = seed;
		}

		void SetType(unsigned char type){
			mType = type;
		}

		int GetBC(){
			mBcSeed = 0x8088405 * mBcSeed + 1;
			return mBcSeed / 0x10000;
		}

		int GetAC(){
			mAcSeed = (0x41C64E6D * mAcSeed + 12345) & 0x7FFFFFFF;
			return mAcSeed;
		}

		int GetMY(){
			mMySeed = 0x41C64E6D * mMySeed + 12345;
			return mMySeed / 0x10000;
		}

		int GetVC(){
			mVcSeed = (0x343FD * mVcSeed + 0x269EC3) & 0x7FFFFFFF;
			return mVcSeed / 0x10000;
		}

		int Get(){
			if(mType == 0)
				return GetBC();
			else if(mType == 1)
				return GetAC();
			else if(mType == 2)
				return GetMY();
			else
				return GetVC();
		}

		unsigned char mType;
		int mVcSeed;
		int mBcSeed;
		int mAcSeed;
		int mMySeed;
	};
};

#endif