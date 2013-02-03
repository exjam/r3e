#ifndef STL_H
#define STL_H

#include "SafeDelete.hpp"
#include "StringMap.hpp"

namespace ROSE {
	class STL {
	private:
		enum {
			STL_TEXT = 1,
			STL_COMMENT = 1 << 1,
			STL_QUEST = 1 << 2,
		} STL_TYPES;

		struct Entry {
			Entry() : mData(0) {}
			~Entry(){
				SAFE_DELETE_ARRAY(mData);
			}

			unsigned int mOffsets[4];
			char* mData;
		};

	public:
		STL();
		STL(const char* path);

		~STL();

		bool Load(const char* path);

		int GetIDByStr(const char* strid) const;

		const char* GetText(unsigned int row) const;
		const char* GetQuest1(unsigned int row) const;
		const char* GetQuest2(unsigned int row) const;
		const char* GetComment(unsigned int row) const;
		const char* GetString(unsigned int row, unsigned int col) const;

	private:
		unsigned int mType;
		StringMap<int> mStrIndex;
		Entry* mEntries;
	};
};

#endif