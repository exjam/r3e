#ifndef HIM_H
#define HIM_H

#include "String.hpp"

namespace ROSE {
	class HIM {
	public:
		HIM();
		~HIM();

		bool Load(const char* path);
		void Unload();

		int Width() const;
		int Height() const;

		float GetHeight(int x, int y) const;
		float** Heights() const;

	private:
		int mWidth;
		int mHeight;
		int mGridCount;
		float mGridSize;
		float mHeights[65][65];
	};
};

#endif