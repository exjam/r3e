#ifndef SMART_POINTER_H
#define SMART_POINTER_H

#include "SmartObject.hpp"

template<class T> class SmartPointer {
public:
	SmartPointer(){
		mObject = 0;
	}

	SmartPointer(SmartObject& obj){
		mObject = &obj;
		if(mObject) mObject->AddReference();
	}

	SmartPointer(SmartObject* obj){
		mObject = obj;
		if(mObject) mObject->AddReference();
	}

	~SmartPointer(){
		if(mObject)
			mObject->DeleteReference();
	}

	T* operator->() const {
		if(!mObject) return NULL;
		return (T*)mObject->Get();
	}

	T* Get() const {
		return (T*)mObject->Get();
	}

	bool operator!=(const SmartPointer<T>& rhs){
		return mObject != rhs.mObject;
	}

	bool operator==(const SmartPointer<T>& rhs){
		return mObject == rhs.mObject;
	}

	SmartPointer<T>& operator=(const SmartPointer<T>& rhs){
		if(mObject) mObject->DeleteReference();
		mObject = rhs.mObject;
		if(mObject) mObject->AddReference();
		return (*this);
	}

private:
	SmartObject* mObject;
};

#endif