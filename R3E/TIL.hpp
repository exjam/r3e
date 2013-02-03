#ifndef TIL_H
#define TIL_H

#include "String.hpp"

namespace ROSE {
	class TIL {
	private:
		struct Tile {
			unsigned char mBrushNumber;
			unsigned char mTileIndex;
			unsigned char mTileSet;
			unsigned int mTileNumber;
		};

	public:
		TIL();
		~TIL();

		bool Load(const char* path);
		void Unload();

		int Width() const;
		int Height() const;

		int GetTile(int x, int y) const;

	private:
		int mWidth;
		int mHeight;
		Tile mTileMap[16][16];
	};
};

#endif