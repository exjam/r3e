#ifndef IFO_HPP
#define IFO_HPP

#include "LIT.hpp"
#include "Array.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"
#include "Quaternion.hpp"
#include "ObjectEntity.hpp"

namespace ROSE {
	class IFO {
	public:
		enum BlockType {
			ECONOMY_DATA = 0,
			DECORATIONS,
			NPC_SPAWNS,
			BUILDINGS,
			SOUND_EFFECTS,
			EFFECTS,
			ANIMATABLES,
			WATER_BIG,
			MONSTER_SPAWNS,
			WATER_PLANES,
			WARP_GATES,
			COLLISION_BLOCK,
			TRIGGERS,
			MAX_BLOCK_TYPE,
		};

	public:
		IFO();
		~IFO();

		bool Load(const char* path);

		void AddObjectsToScene(SceneManager* scene);
		void LinkLightmaps(ROSE::LIT* deco, ROSE::LIT* cnst);

	private:
		void ReadBlock(File* fh, unsigned int type);

	public:
		Array<ObjectEntity*> mObjects;
		Array<ObjectEntity*> mBuildings;
	};
};

#endif