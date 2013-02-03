#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <assert.h>
#include "SafeDelete.hpp"

template <class T> class List {
public:
	class Node {
	public:
		Node() : mNext(0) {}
		Node(T value, Node* next) : mValue(value), mNext(next) {}

		T mValue;
		Node* mNext;
	};

	class Iterator {
	public:
		Iterator(Node* node = 0) : mNode(node) {}

		bool valid() const {
			return mNode != 0;
		}

		Iterator& operator++(){
			if(mNode)
				mNode = mNode->mNext;
			return (*this);
		}

		bool operator==(const Iterator& other){
			return other.mNode == mNode;
		}

		bool operator!=(const Iterator& other){
			return other.mNode != mNode;
		}

		T& operator*() const {
			return mNode->mValue;
		}

	protected:
		Node* GetNode(){
			return mNode;
		}

		friend class List;

	private:
		Node* mNode;
	};

public:
	List() : mFirst(0), mLast(0), mCount(0) {}
	~List(){
		clear();
	}

	Iterator begin() const {
		if(!mFirst) return end();
		return Iterator(mFirst);
	}

	Iterator end() const {
		return 0;
	}

	Iterator push_front(T value){
		Node* node = new Node(value, mFirst);
		mFirst = node;
		++mCount;
		return Iterator(node);
	}

	Iterator push_back(T value){
		Node* node = new Node(value, 0);
		if(!mFirst) mFirst = node;
		else mLast->mNext = node;

		mLast = node;
		++mCount;
		return Iterator(node);
	}

	T pop_front(){
		Node* node = mFirst;
		mFirst = node->mNext;
		T val = node->mValue;
		delete node;
		--mCount;
		return val;
	}

	T& at(unsigned int pos) const {
		assert(pos < size());

		Iterator itr = begin();
		for(unsigned int i = 0; i < pos; ++i, ++itr);
		return *itr;
	}

	void erase(Iterator pos){
		Node* remNode = pos.GetNode();
		if(pos == begin()){
			mFirst = remNode->mNext;
			--mCount;
			if(mCount == 0) mLast = mFirst;
			delete remNode;
			return;
		}

		for(Iterator itr = begin(); itr != end(); ++itr){
			if(itr.GetNode()->mNext != remNode) continue;
			itr.GetNode()->mNext = remNode->mNext;
			--mCount;
			if(remNode == mLast)
				mLast = itr.GetNode();

			delete remNode;
			return;
		}
	}

	Iterator find(T value) const {
		for(Iterator itr = begin(); itr != end(); ++itr){
			if((*itr) == value) return itr;
		}

		return end();
	}

	unsigned int size() const {
		return mCount;
	}

	void clear(){
		if(!mFirst) return;
		Node* cur = mFirst;
		Node* next = mFirst;
		while(next && cur){
			next = cur->mNext;
			delete cur;
			cur = next;
		}
		mFirst = 0;
		mLast = 0;
		mCount = 0;
	}

	void delete_values(bool isArray = false){
		if(!mFirst) return;
		Node* cur = mFirst;
		Node* next = mFirst;
		while(next && cur){
			next = cur->mNext;

			if(isArray){
				SAFE_DELETE_ARRAY(cur->mValue);
			}else{
				SAFE_DELETE(cur->mValue);
			}

			delete cur;
			cur = next;
		}

		mFirst = 0;
		mLast = 0;
		mCount = 0;
	}

private:
	unsigned int mCount;
	Node* mFirst;
	Node* mLast;
};

#endif
