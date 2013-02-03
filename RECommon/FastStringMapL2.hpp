#ifndef FAST_STRING_MAP_L1_H
#define FAST_STRING_MAP_L1_H

#include "SafeDelete.hpp"
#include "String.hpp"
#include "Hash.hpp"

#ifndef ENTRY_LIST_STEP_SIZE
# define ENTRY_LIST_STEP_SIZE 5
#endif

template<class T> class FastStringMapL2 {
public:
	class Entry {
	public:
		Entry(){}
		Entry(Hash hash, T value) : mHash(hash), mValue(value) {}

		Hash mHash;
		T mValue;

		T GetValue() const {
			return mValue;
		}
	};

private:
	class List {
	public:
		List() : mEntries(0), mCount(0), mArraySize(0) {}
		~List(){
			if(!mEntries) return;
			for(unsigned int i = 0; i < mCount; ++i)
				delete mEntries[i];

			delete [] mEntries;
		}

		void AddEntry(Entry* entry){
			CheckArraySize();
			mEntries[mCount] = entry;
			++mCount;
		}

		void CheckArraySize(){
			if(mCount >= mArraySize)
				mArraySize += ENTRY_LIST_STEP_SIZE;
			else return;

			Entry** oldArray = mEntries;
			mEntries = new Entry*[mArraySize];
			memcpy_s(mEntries, sizeof(Entry*) * mArraySize, oldArray, sizeof(Entry*) * mCount);
			memset(mEntries + mCount, 0, sizeof(Entry*) * ENTRY_LIST_STEP_SIZE);
		}

		Entry* GetEntry(Hash hash) const {
			for(unsigned int i = 0; i < mCount; ++i){
				if(mEntries[i]->mHash.mHash == hash.mHash) return mEntries[i];
			}
			return NULL;
		}

		bool RemoveEntry(Hash hash){
			for(unsigned int i = 0; i < mCount; ++i){
				if(mEntries[i]->mHash.mHash != hash.mHash) continue;
				delete mEntries[i];
				mEntries[i] = 0;
				return true;
			}

			return false;
		}

		void DeleteValues(){
			for(unsigned int i = 0; i < mCount; ++i){
				SAFE_DELETE(mEntries[i]->mValue);
			}
		}

		unsigned int mArraySize;
		unsigned int mCount;
		Entry** mEntries;
	};

	class Node {
	public:
		List mChildren[256];
	};
	
public:
	FastStringMapL2(){}
	~FastStringMapL2(){}

	Entry* Add(const char* keyV, T value){
		String key;
		CleanString(keyV, key);
		Hash hash = Hash::Generate(key);
		if(GetEntry(hash)) return NULL;
		Entry* entry = new Entry(hash, value);
		AddEntry(entry);
		return entry;
	}

	T& operator[](const char* keyV){
		Entry* entry = find(key);
		if(!entry)
			entry = add(key, T());

		return entry->mValue;
	}

	Entry* Find(const char* keyV) const {
		String key;
		CleanString(keyV, key);
		Hash hash = Hash::Generate(key);
		return GetEntry(hash);
	}

	void DeleteValues(){
		for(int x = 0; x < 256; ++x){
			for(int y = 0; y < 256; ++y){
				mKeyNodes[x].mChildren[y].DeleteValues();
			}
		}
	}

private:
	Entry* GetEntry(Hash hash) const {
		return mKeyNodes[hash.keys.mBranch1].mChildren[hash.keys.mBranch2].GetEntry(hash);
	}

	void AddEntry(Entry* entry){
		mKeyNodes[entry->mHash.keys.mBranch1].mChildren[entry->mHash.keys.mBranch2].AddEntry(entry);
	}

	bool RemoveEntry(Hash hash){
		return mKeyNodes[hash.keys.mBranch1].mChildren[hash.keys.mBranch2].RemoveEntry(hash);
	}

	void CleanString(const char* in, String& out) const {
		out = in;
		char* pch = const_cast<char*>((const char*)out);
		int pos = 0;
		while(*pch){
			char cChar = *pch;
			if(cChar == '/') *pch = '\\';
			if(cChar == '\\' && *(pch + 1) == '\\') memcpy_s(pch, 255 - pos, pch + 1, 254 - pos);
			if(cChar >= 'a' && cChar <= 'z') *pch = cChar - 32;
			++pch;
			++pos;
		}
	}

	Node mKeyNodes[256];
};

#endif