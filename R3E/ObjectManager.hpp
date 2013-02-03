#ifndef OBJECT_MAN_HPP
#define OBJECT_MAN_HPP

class Mesh1TexEntity;

namespace ROSE {
	template<class T> class ZSC;
};

class ObjectManager {
private:
	static ObjectManager mInstance;

public:
	ObjectManager();
	~ObjectManager();

	static ObjectManager& Instance(){
		return mInstance;
	}

	void SetCnst(const char* path);
	void SetDeco(const char* path);
	
	EntityGroup* LoadObject(int type, int id) const;

private:
	ROSE::ZSC<Mesh1TexEntity>* mDeco;
	ROSE::ZSC<Mesh1TexEntity>* mCnst;
};

#endif