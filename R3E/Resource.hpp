#ifndef RESOURCE_H
#define RESOURCE_H

#include "SmartObject.hpp"
#include "String.hpp"

class Resource : public SmartObject {
public:
	Resource() : mResourceLoaded(false) {}
	virtual ~Resource(){}

	void Create(const char* path){
		mResourcePath = path;
	}

	virtual void Destroy(){
		Unload();
		mResourceLoaded = false;
	}

	virtual void* Get(){
		LoadResource();
		return this;
	}

	virtual bool Load(const char* path) = 0;
	virtual void Unload() = 0;

private:
	void LoadResource(){
		if(mResourceLoaded) return;
		Load(mResourcePath);
		mResourceLoaded = true;
	}

private:
	String mResourcePath;
	bool mResourceLoaded;
};

#endif