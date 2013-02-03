#ifndef EVENT_QUEUE_HPP
#define EVENT_QUEUE_HPP

#include "Pool.hpp"
#include "Array.hpp"
#include "CriticalSection.hpp"

template<class EvtType> class EventQueue {
public:
	EventQueue() : mPollIndex(0) {}
	~EventQueue(){
		mEvents.Clear();
	}

	bool StartPoll(){
		mSection.Enter();
		if(mEvents.Size() != 0)
			return true;
		
		mSection.Leave();
		return false;
	}

	EvtType* Poll(){
		if(mPollIndex >= mEvents.Size()) return NULL;
		return mEvents[mPollIndex++];
	}

	void EndPoll(){
		mPollIndex = 0;

		for(unsigned int i = 0; i < mEvents.Size(); ++i)
			FreeEvent(mEvents[i]);

		mEvents.Clear();
		mSection.Leave();
	}

	EvtType* CreateEvent(){
		mSection.Enter();
		EvtType* data = mDataPool.Alloc();
		mSection.Leave();
		return data;
	}

	void Enqueue(EvtType* evt){
		mSection.Enter();
		mEvents.PushBack(evt);
		mSection.Leave();
	}

private:
	void FreeEvent(EvtType* evt){
		mSection.Enter();
		mDataPool.Free(evt);
		mSection.Leave();
	}

private:
	CriticalSection mSection;

	Pool<EvtType> mDataPool;
	Array<EvtType*, 50> mEvents;

	unsigned int mPollIndex;
};

#endif