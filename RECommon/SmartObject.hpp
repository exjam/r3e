#ifndef SMART_OBJECT_H
#define SMART_OBJECT_H

class SmartObject {
public:
	SmartObject() : mReferences(0) {}
	virtual ~SmartObject(){}

	void AddReference(){
		++mReferences;
	}

	void DeleteReference(){
		--mReferences;
		if(mReferences == 0) Destroy();
	}

	virtual void Destroy() = 0;
	virtual void* Get() = 0;

private:
	int volatile mReferences;
};

#endif