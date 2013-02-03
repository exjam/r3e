#ifndef READFILE_H
#define READFILE_H

inline long ReadWholeFile(char* &buffer, const char* path){
	buffer = 0;
	FILE* fh;
	fopen_s(&fh, path, "rb");
	if(!fh) return 0;
	fseek(fh, 0, SEEK_END);
	long size = ftell(fh);
	fseek(fh, 0, SEEK_SET);
	buffer = new char[size+1];
	fread(buffer, size, 1, fh);
	fclose(fh);
	buffer[size] = 0;
	return size;
}

#endif
