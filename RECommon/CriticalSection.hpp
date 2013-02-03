#ifndef CRITICAL_SECTION_HPP
#define CRITICAL_SECTION_HPP

#include "Windows.hpp"

class CriticalSection {
public:
	CriticalSection(){
		InitializeCriticalSection(&mSection);
	}

	~CriticalSection(){
		DeleteCriticalSection(&mSection);
	}

	void Enter(){
		EnterCriticalSection(&mSection);
	}

	bool TryEnter(){
		return TryEnterCriticalSection(&mSection) == 0;
	}

	void Leave(){
		LeaveCriticalSection(&mSection);
	}

protected:
	CRITICAL_SECTION mSection;
};

#endif