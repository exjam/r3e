#ifndef ARRAY_H
#define ARRAY_H

#include <string.h>
#include <xmemory>
#include "SafeDelete.hpp"

template <class T, int INC_ARRAY_SIZE = 1> class Array {
public:
	static const unsigned int NOT_FOUND = 0xFFFFFFFF;

	Array() : mData(0), mSize(0), mMaxSize(0) {}
	~Array(){
		Delete();
	}

	void Clear(){
		T* iptr = mData;
		for(unsigned int i = 0; i < mSize; ++i, ++iptr)
			mAllocator.destroy(iptr);

		memset(mData, 0, mSize * sizeof(T));
		mSize = 0;
	}

	void Delete(){
		if(mData) destroyArray(mData, mSize, mMaxSize);
		mData = 0;
		mSize = 0;
		mMaxSize = 0;
	}

	void PushFront(const T& value){
		Insert(0, value);
	}

	T& Add(){
		if(!mData || mSize == mMaxSize)
			Resize(mSize + 1);

		++mSize;
		T& val = mData[mSize - 1];
		mAllocator.construct(&val, T());
		return val;
	}

	void PushBack(const T& value){
		if(!mData || mSize == mMaxSize)
			Resize(mSize + 1);

		mData[mSize] = value;
		++mSize;
	}

	template<int INC_ARRAY_SIZE_2> void Append(const Array<T, INC_ARRAY_SIZE_2>& other){
		Append(other.Data(), other.Size());
	}

	void Append(const T* values, unsigned int count){
		if(count == 0) return;
		Resize(Size() + count);
		memcpy_s(&mData[mSize], sizeof(T) * (mMaxSize - mSize), values, sizeof(T) * count);
		mSize += count;
	}

	void Insert(unsigned int idx, const T& value){
		Insert(idx, &value, 1);
	}

	template<int INC_ARRAY_SIZE_2> void Insert(unsigned int idx, const Array<T, INC_ARRAY_SIZE_2>& other){
		Insert(idx, other.Data(), other.Size());
	}

	void Insert(unsigned int idx, const T* values, unsigned int count){
		if(count == 0) return;
		if(idx > mSize) return;

		if(!mData || mSize + count >= mMaxSize)
			Resize(mSize + count);

		memmove_s(&mData[idx + count], (mMaxSize - idx - count) * sizeof(T), &mData[idx], (mSize - idx) * sizeof(T));
		memcpy_s(&mData[idx], mMaxSize - idx, values, count * sizeof(T));

		mSize += count;
	}
	
	void Erase(int idx, unsigned int count = 1){
		if(count == 0) return;
		if(!mData) return;
		if(mSize <= count) return Clear();
		if(idx == NOT_FOUND) return;
		if(idx + count > mSize) return;

		int size = sizeof(T) * (mMaxSize - idx);
		int bytes = size - (sizeof(T) * count);

		memmove_s(&mData[idx], size, &mData[idx + count], bytes);
		mSize -= count;
	}

	unsigned int Find(T value) const {
		for(unsigned int i = 0; i < Size(); ++i)
			if(mData[i] == value)
				return i;

		return NOT_FOUND;
	}

	unsigned int FindReverse(T value, unsigned int from) const {
		for(unsigned int i = from; i >= 0; --i)
			if(mData[i] == value)
				return i;

		return NOT_FOUND;
	}

	unsigned int Find(T value, unsigned int from) const {
		for(unsigned int i = from; i < Size(); ++i)
			if(mData[i] == value)
				return i;

		return NOT_FOUND;
	}

	T& At(unsigned int idx) const {
		return mData[idx];
	}

	T& operator[] (unsigned int idx) const {
		return mData[idx];
	}

	void DeleteValues(bool isArray = false){
		if(!mData) return;

		for(unsigned int i = 0; i < Size(); ++i){
			if(isArray){
				SAFE_DELETE_ARRAY(mData[i]);
			}else{
				SAFE_DELETE(mData[i]);
			}
		}

		Clear();
	}

	T* Data() const {
		return mData;
	}

	unsigned int Size() const {
		return mSize;
	}

	void SetSize(int size){
		Resize(size);
		mSize = size;
	}

	unsigned int MaxSize() const {
		return mMaxSize;
	}

	void Resize(unsigned int size){
		if(!mData){
			size = ((size + INC_ARRAY_SIZE) / INC_ARRAY_SIZE) * INC_ARRAY_SIZE;
			mData = allocArray(size + 1);
			memset(mData, 0, sizeof(T) * size);
			mMaxSize = size;
			return;
		}

		if(!size){
			Clear();
			return;
		}

		if(size == mMaxSize) return;

		if(size > mMaxSize){
			size = ((size + INC_ARRAY_SIZE) / INC_ARRAY_SIZE) * INC_ARRAY_SIZE;
			T* newData = allocArray(size + 1);
			memcpy_s(newData, sizeof(T) * size, mData, sizeof(T) * mSize);
			memset(&newData[mSize], 0, (size - mSize) * sizeof(T));
			deallocArray(mData, mMaxSize);
			mData = newData;
			mMaxSize = size;
		}else if(size < mSize){
			memset(&mData[size], 0, mSize - size);
			mSize = size;
		}
	}

private:
	T* allocArray(unsigned int count){
		if(!count) return 0;
		return mAllocator.allocate(count, 0);
	}

	void deallocArray(T* ptr, unsigned int count){
		if(!ptr || !count) return;
		mAllocator.deallocate(ptr, count);
	}

	void destroyArray(T* ptr, unsigned int destruct, unsigned int dealloc){
		if(!ptr || !dealloc) return;
		T* iptr = ptr;
		for(unsigned int i = 0; i < destruct; ++i, ++iptr)
			mAllocator.destroy(iptr);

		mAllocator.deallocate(ptr, dealloc);
	}

private:
	T* mData;
	unsigned int mSize;
	unsigned int mMaxSize;
	std::allocator<T> mAllocator;
};

#endif

/*#ifndef ARRAY_H
#define ARRAY_H

#include <string.h>
#include "SafeDelete.hpp"

template <class T, int INC_ARRAY_SIZE = 1> class Array {
public:
	static const unsigned int NOT_FOUND = 0xFFFFFFFF;

	Array() : mData(0), mCount(0), mMaxSize(0) {}
	~Array(){
		clear();
	}

	void clear(){
		SAFE_DELETE_ARRAY(mData);
		mCount = 0;
		mMaxSize = 0;
	}

	void move_to_end(int idx){
		if(!mData) return;
		if(mCount <= 1) return;
		if(idx == NOT_FOUND) return;
		if(idx >= (mCount - 1)) return;

		T temp = mData[idx];

		int sizeofRemainingMemory = sizeof(T) * (mMaxSize - idx);
		int sizeofCopyMemory = sizeofRemainingMemory - 4;
		memcpy_s(&mData[idx], sizeofRemainingMemory, &mData[idx + 1], sizeofCopyMemory);
		mData[mCount - 1] = temp;
	}
	
	void erase(int idx){
		if(!mData) return;
		if(mCount == 1) return clear();
		if(idx == NOT_FOUND) return;
		if(idx >= mCount) return;

		int sizeofRemainingMemory = sizeof(T) * (mMaxSize - idx);
		int sizeofCopyMemory = sizeofRemainingMemory - 4;

		memcpy_s(&mData[idx], sizeofRemainingMemory, &mData[idx + 1], sizeofCopyMemory);
		--mCount;
		//mData[mCount] = 0;
	}

	void push_front(T value){
		if(!mData){
			mData = new T[INC_ARRAY_SIZE];
		}else{
			if(mCount < mMaxSize){
				memcpy_s(mData + sizeof(T), sizeof(T) * mMaxSize, mData, sizeof(T) * mCount);
			}else{
				mMaxSize = mCount + INC_ARRAY_SIZE;
				T* newData = new T[mMaxSize];
				memcpy_s(newData + sizeof(T), sizeof(T) * mMaxSize, mData, sizeof(T) * mCount);
				mData = newData;
			}
		}

		++mCount;
		mData[0] = value;
	}

	void push_back(T value){
		if(!mData)
			mData = new T[INC_ARRAY_SIZE];

		if(mCount == mMaxSize){
			resize(mCount + INC_ARRAY_SIZE);
		}

		mData[mCount] = value;
		++mCount;
	}

	void setCount(int count){
		resize(count);
		mCount = count;
	}

	void resize(int size){
		if(!mData){
			mData = new T[size];
			memset(mData, 0, sizeof(T) * size);
			mMaxSize = size;
			return;
		}

		if(!size){
			clear();
			return;
		}

		if(size == mMaxSize) return;

		if(size > mMaxSize){
			T* newData = new T[size];
			memcpy_s(newData, sizeof(T) * size, mData, sizeof(T) * mCount);
			memset(newData + (sizeof(T) * mCount), 0, (mMaxSize - mCount) * sizeof(T));

			mData = newData;
			mMaxSize = size;
		}else{
			memset(mData + (sizeof(T) * mCount), 0, mCount - size);
			mCount = size;
			mMaxSize = size;
		}
	}

	T& operator[] (unsigned int idx) const {
		return mData[idx];
	}

	unsigned int find(T value) const {
		for(unsigned int i = 0; i < size(); ++i){
			if(mData[i] == value) return i;
		}

		return NOT_FOUND;
	}

	unsigned int size() const {
		return mCount;
	}

	unsigned int maxSize() const {
		return mMaxSize;
	}

	void delete_values(bool isArray = false){
		if(!mData) return;

		for(int i = 0; i < mCount; ++i){
			if(isArray){
				SAFE_DELETE_ARRAY(mData[i]);
			}else{
				SAFE_DELETE(mData[i]);
			}
		}

		clear();
	}

	T* GetData() const {
		return mData;
	}

private:
	T* mData;
	int mCount;
	int mMaxSize;
};

#endif*/