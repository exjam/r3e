#ifndef FLAT_FILE_H
#define FLAT_FILE_H

#include "File.hpp"
#include <stdio.h>

class FlatFile : public File {
public:
	FlatFile() : fh(0) {}

	FlatFile(const char* path, const char* method){
		Open(path, method);
	}

	~FlatFile(){
		Close();
	}

	bool Open(const char* path, const char* method){
		fopen_s(&fh, path, method);
		return IsOpen();
	}

	bool IsOpen(){
		return (fh != NULL);
	}

	void Close(){
		if(!fh) return;
		fclose(fh);
	}

	int ReadData(void* data, int size){
		return fread(data, size, 1, fh);
	}

	int WriteData(void* data, int size){
		return fwrite(data, size, 1, fh);
	}

	void Skip(int bytes){
		fseek(fh, bytes, SEEK_CUR);
	}

	void Seek(int position){
		fseek(fh, position, SEEK_SET);
	}

	long Position(){
		return ftell(fh);
	}

	long Size(){
		long pos = Position();
		fseek(fh, 0, SEEK_END);
		long pos2 = Position();
		Seek(pos);
		return pos2;
	}

	bool IsEOF(){
		return feof(fh) != 0;
	}

private:
	FILE* fh;
};

#endif
