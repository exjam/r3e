#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>

#include "Windows.hpp"

enum LogMethod {
	NO_LOG,
	LOG_CONSOLE,
	LOG_FILE,
};

class Log {
public:
	Log() : mLogMethod(NO_LOG), mFh(0), mLogFile("r3e.log") {}
	~Log(){}

	void SetLogMethod(int method){
		if(mLogMethod == method) return;
		mLogMethod = method;

		if(method == LOG_FILE){
			CreateFile();
			DestroyConsole();
		}else if(method == LOG_CONSOLE){
			CreateConsole();
			DestroyFile();
		}
	}

	void LogMessage(char* format, ...){
	   if(mLogMethod == NO_LOG) return;
	   va_list args;
	   va_start(args, format);
	   vsprintf_s(mLogBuffer, 1024, format, args);
	   if(mLogMethod == LOG_CONSOLE)
		   LogConsole(mLogBuffer);
	   else if(mLogMethod == LOG_FILE)
		   LogFile(mLogBuffer);
	}

	static Log& Instance(){
		return mInstance;
	}

private:
	void LogConsole(const char* msg){
		printf(msg);
		printf("\n");
	}

	void LogFile(const char* msg){
		fprintf(mFh, msg);
		fprintf(mFh, "\n");
	}

	void CreateFile(){
		fopen_s(&mFh, mLogFile, "a+");
	}

	void CreateConsole(){
		FILE* console;
		AllocConsole();
		SetConsoleTitleA("R3E Console");
		freopen_s(&console, "CONOUT$", "wb", stdout);
		LogMessage("Created R3E Console");
	}

	void DestroyFile(){
		if(mFh) fclose(mFh);
	}

	void DestroyConsole(){
		//FreeConsole();
	}

private:
	static char mLogBuffer[1024];
	static Log mInstance;
	const char* mLogFile;
	int mLogMethod;
	FILE* mFh;
};

#define LOGMODE Log::Instance().SetLogMethod
#define LOG Log::Instance().LogMessage

#endif