#include "Global.h"

#include "Log.hpp"
#include "STB.hpp"
#include "FileSystem.hpp"
#include "SafeDelete.hpp"
#include "ScopedPointer.hpp"

namespace ROSE {
	STB::STB() : mDataOffsets(0), mStbData(0), mColCount(0), mRowCount(0) {}
	STB::STB(const char* path) : mDataOffsets(0), mStbData(0), mColCount(0), mRowCount(0) {
		Load(path);
	}

	STB::~STB(){
		SAFE_DELETE_ARRAY(mStbData);
		SAFE_DELETE_ARRAY(mDataOffsets);
	}

	bool STB::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;

		char VersionHeader[3];
		fh->ReadData(VersionHeader, 3);
		if(strncmp(VersionHeader, "STB", 3)){
			LOG("Error: '%s' invalid version '%s'", path, VersionHeader);
			fh->Close();
			return false;
		}

		unsigned int offset;
		unsigned short len;

		fh->Skip(1);
		fh->Read(offset);
		fh->Read(mRowCount);
		fh->Read(mColCount);
		fh->Seek(offset);

		--mRowCount;
		--mColCount;

		mBytes = fh->Size() - fh->Position();
		long dataPos = fh->Position();
		mStbData = new char[mBytes];
		fh->ReadData(mStbData, mBytes);
		fh->Seek(dataPos);
		mDataOffsets = new long[mRowCount * mColCount];
		unsigned int dataIdx = 0;
		for(unsigned int i = 0; i < mRowCount; ++i){
			for(unsigned int j = 0; j < mColCount; ++j){
				fh->Read(len);
				mStbData[fh->Position() - dataPos - 2] = 0;
				mDataOffsets[dataIdx] = fh->Position() - dataPos;
				fh->Skip(len);
				++dataIdx;
			}
		}

		fh->Close();
		return true;
	}

	const char* STB::GetString(int row, int column) const {
		if(row < 0) return NULL;
		if(column < 0) return NULL;
		return mStbData + mDataOffsets[(row*mColCount) + column];
	}

	int STB::GetInt(int row, int column) const {
		return atoi(GetString(row, column));
	}

	unsigned int STB::Rows() const {
		return mRowCount;
	}

	unsigned int STB::Columns() const {
		return mColCount;
	}
};