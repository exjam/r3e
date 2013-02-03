#ifndef LOCKED_POOL_HPP
#define LOCKED_POOL_HPP

#include "Pool.hpp"
#include "CriticalSection.hpp"

template<class T, int ISIZE = 10> class LockedPool : public Pool<T, ISIZE> {
public:
	LockedPool(){}
	~LockedPool(){}

	T* Alloc(){
		mSection.Enter();
		T* val = Pool::Alloc();
		mSection.Leave();
		return val;
	}

	void Free(T* val){
		mSection.Enter();
		Pool::Free(val);
		mSection.Leave();
	}

private:
	CriticalSection mSection;
};


#endif