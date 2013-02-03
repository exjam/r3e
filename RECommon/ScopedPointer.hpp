#ifndef SCOPE_POINTER_H
#define SCOPE_POINTER_H

#include "SafeDelete.hpp"

template <class T> class ScopedPointer {
public:
	ScopedPointer() : mVal(0) {}
	ScopedPointer(T* val) : mVal(val) {}

	~ScopedPointer(){
		SAFE_DELETE(mVal);
	}

	void SetPointer(T* val){
		mVal = val;
	}

	T* operator->() const {
		return mVal;
	}

	bool operator!() const {
		return mVal == NULL;
	}

	operator T*() const {
		return mVal;
	}

private:
	T* mVal;
};

#endif