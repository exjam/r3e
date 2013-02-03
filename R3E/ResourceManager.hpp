#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Resource.hpp"
#include "SmartPointer.hpp"
#include "FastStringMapL2.hpp"
#include "Log.hpp"

#define CLASS_NAME(x) # x

template<class T> class ResourceManager {
public:
	ResourceManager(){}
	~ResourceManager(){
		mResources.DeleteValues();
	}

	static ResourceManager<T>& Instance(){
		return mInstance;
	}
	
	SmartPointer<T> Load(const char* path){
		FastStringMapL2<Resource*>::Entry* entry = mResources.Find(path);
		Resource* res;
		if(!entry){
			res = new T();
			res->Create(path);
			mResources.Add(path, res);
		}else{
			res = entry->GetValue();
		}

		return SmartPointer<T>(res);
	}

private:
	static ResourceManager<T> mInstance;
	FastStringMapL2<Resource*> mResources;
};

#define MESH_MGR() ResourceManager<ROSE::ZMS>::Instance()
#define ANIM_MGR() ResourceManager<ROSE::ZMO>::Instance()
#define SKEL_MGR() ResourceManager<ROSE::ZMD>::Instance()
#define TEX_MGR() ResourceManager<Texture>::Instance()
#define FONT_MGR() ResourceManager<Font>::Instance()

#endif