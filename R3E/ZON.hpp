#ifndef ZON_HPP
#define ZON_HPP

#include "Texture.hpp"
#include "ResourceManager.hpp"

enum BlockType {
	BASIC_DATA = 0,
	EVENT_POINTS,
	TEXTURE_LIST,
	TILE_LIST,
	ECONOMY_DATA,
	BLOCK_TYPE_MAX
};

namespace ROSE {
	class ZON {
	public:
		struct Tile {
			unsigned int mLayer1;
			unsigned int mLayer2;
			unsigned int mOffset1;
			unsigned int mOffset2;
			unsigned int mBlending;
			unsigned int mRotation;
			unsigned int mType;

			void RotateUV(float& x, float& y){
				switch(mRotation){
					case 2:
						x = 1.0f - x;
					return;
					case 3:
						y = 1.0f - y;
					return;
					case 4:
						x = 1.0f - x;
						y = 1.0f - y;
					return;
					case 5:
					{
                        float tmp = x;
                        x = y;
                        y = 1.0f - tmp;
					}
					return;
					case 6:
					{
                        float tmp = x;
                        x = y;
                        y = tmp;
					}
					return;
				}
				return;
			}
		};

	public:
		ZON();
		~ZON();
		
		bool Load(const char* path);

		Tile* GetTile(int id) const;
		SmartPointer<Texture> GetTexture(int id) const;

	private:
		void ReadTileList(File* fh);
		void ReadTextureList(File* fh);

	private:
		Array<SmartPointer<Texture>> mTextureList; 
		Array<Tile> mTileList; 
	};
};

#endif