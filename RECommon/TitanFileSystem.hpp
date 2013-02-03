#ifndef TITAN_FILE_SYSTEM_H
#define TITAN_FILE_SYSTEM_H

#include "String.hpp"
#include "FileSystem.hpp"
#include "TitanFile.hpp"
#include "FlatFile.hpp"
#include "Log.hpp"
#include <stdio.h>

#include "D:\Programming\Projects\TitanVFS\TitanVFS\Index.hpp"

class TitanFileSystem : public FileSystem {
public:
	TitanFileSystem(){}
	TitanFileSystem(const char* baseDir){
		SetBaseDirectory(baseDir);
	}

	virtual ~TitanFileSystem(){}

	bool LoadIndex(const char* path){
		String realpath = path;
		GetFullPath(realpath);

		String trfpath = "data.trf";
		GetFullPath(trfpath);

		return mIndex.Load(realpath, true, false, trfpath);
	}

	virtual File* OpenFile(const char* path, const char* /*mode*/, bool /*dataFile*/ = true, bool /*showError*/ = true){
		FileEntry* entry = 0;

		{
			String tmp = path;
			GetFullPath(tmp);
			FlatFile* file = new FlatFile(tmp, "rb");
			if(!file->IsOpen()){
				file->Open(path, "rb");
				if(!file->IsOpen()){
					delete file;
					entry = mIndex.GetFileByName(path);
					if(!entry)
						return 0;
				}else{
					return file;
				}
			}else{
				return file;
			}
		}

		TitanFile* file = new TitanFile();
		unsigned char* buffer = new unsigned char[entry->mSize];
		file->SetData(buffer, entry->mSize);
		mIndex._fseek(entry->mPosition, FILE_BEGIN);
		mIndex._fread(buffer, entry->mSize);
		return file;
	}

	virtual bool FileExists(const char* path){
		File* fh = OpenFile(path, "rb");
		if(!fh) return false;
		delete fh;
		return true;
	}

	virtual long FileSize(const char* path){
		File* fh = OpenFile(path, "rb");
		if(!fh) return 0;
		long size = fh->Size();
		delete fh;
		return size;
	}

	virtual void GetFullPath(String& path){
		path = mBaseDirectory + path;
	}

	void SetBaseDirectory(const char* dir){
		mBaseDirectory = dir;
		mBaseDirectory.ReplaceAll("\"", "");
		mBaseDirectory.ReplaceAll("/", "\\");

		if(mBaseDirectory.Length() > 0){
			char end = mBaseDirectory.At(mBaseDirectory.Length() - 1);
			if(end != '\\') mBaseDirectory += "\\";
		}

		LOG("FlatFileSystem Base Directory: '%s'", mBaseDirectory);
	}

	const char* GetBaseDirectory(){
		return mBaseDirectory;
	}

private:
	String mBaseDirectory;
	Index mIndex;
};

#endif