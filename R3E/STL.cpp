#include "Global.h"

#include "STL.hpp"
#include "Log.hpp"
#include "FileSystem.hpp"
#include "ScopedPointer.hpp"

namespace ROSE {
	STL::STL() : mEntries(0) {}
	STL::STL(const char* path) : mEntries(0) {
		Load(path);
	}

	STL::~STL(){
		SAFE_DELETE_ARRAY(mEntries);
	}

	bool STL::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;
		char strBuffer[512];
		unsigned char nullChar = 0;

		fh->ReadPascalString(strBuffer);
		if(strcmp(strBuffer, "NRST01") == 0){
			mType = STL_TEXT;
		}else if(strcmp(strBuffer, "ITST01") == 0){
			mType = STL_TEXT | STL_COMMENT;
		}else if(strcmp(strBuffer, "QEST01") == 0){
			mType = STL_TEXT | STL_COMMENT | STL_QUEST;
		}else{
			LOG("Error: '%s' invalid version '%s'", path, strBuffer);
			fh->Close();
			return false;
		}

		unsigned int entryCount;
		long offset1, offset2;

		fh->Read(entryCount);
		mEntries = new Entry[entryCount];

		for(unsigned int i = 0; i < entryCount; ++i){
			fh->ReadPascalString(strBuffer);
			fh->Skip(4);
			mStrIndex.Add(strBuffer, i);//fh->Read<unsigned int>());
		}

		fh->Skip(4 + 4);

		fh->Read(offset1);
		fh->Seek(offset1);

		//Method below is optimised for buffered file system
		//Also reduces memory allocations for increased speed
		for(unsigned int i = 0; i < entryCount; ++i){
			unsigned int len, totalLen = 0;
			fh->Read(offset2);
			offset1 = fh->Position();
			fh->Seek(offset2);

			Entry* entry = &mEntries[i];
			len = fh->ReadPascalStringLen();
			entry->mOffsets[0] = fh->Position() - offset2;
			fh->Skip(len);

			totalLen = entry->mOffsets[0] + len;
			if(mType & STL_COMMENT){
				len = fh->ReadPascalStringLen();
				fh->WriteData(&nullChar, 1, totalLen + offset2);
				entry->mOffsets[1] = fh->Position() - offset2;
				fh->Skip(len);
				totalLen = len + entry->mOffsets[1];
			}

			if(mType & STL_QUEST){
				len = fh->ReadPascalStringLen();
				fh->WriteData(&nullChar, 1, totalLen + offset2);
				entry->mOffsets[2] = fh->Position() - offset2;
				fh->Skip(len);
				totalLen = len + entry->mOffsets[2];

				len = fh->ReadPascalStringLen();
				fh->WriteData(&nullChar, 1, totalLen + offset2);
				entry->mOffsets[3] = fh->Position() - offset2;
				fh->Skip(len);
				fh->WriteData(&nullChar, 1);

				totalLen = len + entry->mOffsets[3];
			}

			entry->mData = new char[totalLen+1];
			fh->Seek(offset2);
			fh->ReadData(entry->mData, totalLen);
			entry->mData[totalLen] = 0;

			fh->Seek(offset1);
		}

		fh->Close();
		return true;
	}

	int STL::GetIDByStr(const char* strid) const {
		StringMap<int>::Iterator itr = mStrIndex.Find(strid);
		if(itr == mStrIndex.End()) return -1;
		return *itr;
	}

	const char* STL::GetText(unsigned int row) const {
		return GetString(row, 0);
	}

	const char* STL::GetComment(unsigned int row) const {
		return GetString(row, 1);
	}

	const char* STL::GetQuest1(unsigned int row) const {
		return GetString(row, 2);
	}

	const char* STL::GetQuest2(unsigned int row) const {
		return GetString(row, 3);
	}

	const char* STL::GetString(unsigned int row, unsigned int col) const {
		return mEntries[row].mData + mEntries[row].mOffsets[col];
	}

};