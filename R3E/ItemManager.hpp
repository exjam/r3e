#ifndef ITEM_MAN_H
#define ITEM_MAN_H

#include "STB.hpp"
#include "STL.hpp"
#include "RoseConstants.hpp"
#include "Mesh1TexEntity.hpp"
#include "ZSC.hpp"

#include "MotionManager.hpp"

class ItemManager {
private:
	static ItemManager mInstance;

public:
	ItemManager();
	~ItemManager();

	static ItemManager& Instance(){
		return mInstance;
	}

	void Load();

	Entity* LoadItem(int id, int type, int gender, Entity* parent = 0) const;

	ROSE::STB* GetSTB(int type) const;
	ROSE::STL* GetSTL(int type) const;

private:
	Array<ROSE::ZSC<Mesh1TexEntity>*> mItemModels[G_MAX];
	Array<ROSE::STB*> mItemSTB;
	Array<ROSE::STL*> mItemSTL;
};

#endif