#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "File.hpp"

class FileSystem {
public:
	FileSystem(){}
	virtual ~FileSystem(){}

	virtual File* OpenFile(const char* path, const char* mode, bool dataFile = true, bool showError = true) = 0;

	virtual bool FileExists(const char* path) = 0;
	virtual long FileSize(const char* path) = 0;

	virtual void GetFullPath(String& path) = 0;

	static void SetFileSystem(FileSystem* sys){
		mFileSystem = sys;
	}

	static FileSystem* GetFileSystem(){
		return mFileSystem;
	}

private:
	static FileSystem* mFileSystem;
};

#define FILE_SYS() FileSystem::GetFileSystem()

#endif