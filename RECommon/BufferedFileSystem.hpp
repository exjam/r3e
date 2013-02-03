#ifndef BUFFERED_FILE_SYSTEM_H
#define BUFFERED_FILE_SYSTEM_H

#include "String.hpp"
#include "FileSystem.hpp"

#include "BufferedFile.hpp"
#include "Log.hpp"

class BufferedFileSystem : public FileSystem {
public:
	BufferedFileSystem(){}
	BufferedFileSystem(const char* baseDir){
		SetBaseDirectory(baseDir);
	}

	virtual ~BufferedFileSystem(){}

	virtual File* OpenFile(const char* path, const char* mode, bool dataFile = true, bool showError = true){
		String modeStr = mode;
		if(modeStr.Find("w") != 0xFFFFFFFF){
			if(showError) LOG("Error: BufferedFileSystem does not support file writing! ('%s' [%s])", path, mode);
			return NULL;
		}

		String realpath = path;
		if(dataFile) GetFullPath(realpath);

		BufferedFile* file = new BufferedFile();
		if(!file->Open(realpath, mode)){
			if(showError) LOG("Error: BufferedFileSystem could not open file: '%s' [%s]", path, mode);
			delete file;
			return NULL;
		}

		return file;
	}

	virtual bool FileExists(const char* path){
		File* fh = OpenFile(path, "rb", true, false);
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
		char end = mBaseDirectory.At(mBaseDirectory.Length() - 1);
		if(end != '\\') mBaseDirectory += "\\";
		LOG("BufferedFileSystem Base Directory: '%s'", mBaseDirectory);
	}

	const char* GetBaseDirectory(){
		return mBaseDirectory;
	}

private:
	String mBaseDirectory;
};

#endif