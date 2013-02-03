#ifndef POOL_HPP
#define POOL_HPP

#include "Array.hpp"
#include <xmemory>

template<class T, int ISIZE = 10> class Pool {
private:
	struct PoolMem {
		bool mAllocated;
		T* mValue;
	};

public:
	Pool(){}
	~Pool(){
		for(unsigned int i = 0; i < mDataPool.Size(); ++i){
			delete mDataPool[i].mValue;
		}

		mDataPool.Clear();
	}

	T* Alloc(){
		for(unsigned int i = 0; i < mDataPool.Size(); ++i){
			PoolMem& mem = mDataPool[i];
			if(mem.mAllocated) continue;
			mem.mAllocated = true;
			return mem.mValue;
		}

		PoolMem& mem = mDataPool.Add();
		mem.mAllocated = true;
		mem.mValue = new T();
		return mem.mValue;
	}

	void Free(T* val){
		for(unsigned int i = 0; i < mDataPool.Size(); ++i){
			T* ptr = mDataPool[i].mValue;
			if(ptr != val) continue;
			mDataPool[i].mAllocated = false;

			mAllocator.destroy(ptr);
			break;
		}
	}

private:
	Array<PoolMem, ISIZE> mDataPool;
	std::allocator<T> mAllocator;
};

#endif