#ifndef CHARACTER_LIST_HPP
#define CHARACTER_LIST_HPP

#include "Array.hpp"
#include "Character.hpp"

class CharacterList {
public:
	CharacterList(){
		Clear();
	}

	~CharacterList(){
		mList.Clear();
	}

	void Add(Character* chr){
		mList.PushBack(chr);
	}

	void Draw2D(){
		for(unsigned int i = 0; i < mList.Size(); ++i)
			mList[i]->Draw2D();
	}

	void Remove(Character* chr){
		Remove(chr->GetClientID());
	}

	void Remove(unsigned int clientid){
		for(unsigned int i = 0; i < mList.Size(); ++i){
			if(mList[i]->GetClientID() != clientid) continue;
			mList.Erase(i);
			return;
		}
	}

	Character* Get(unsigned int clientid){
		for(unsigned int i = 0; i < mList.Size(); ++i){
			if(mList[i]->GetClientID() == clientid){
				return mList[i];
			}
		}
		
		return NULL;
	}

	void Clear(){
		mList.Clear();
	}

private:
	Array<Character*, 50> mList;
};

extern CharacterList* gCharacterList;

#endif