#ifndef MAP_HPP
#define MAP_HPP

#include "Range.hpp"
#include "Entity.hpp"

namespace ROSE {
	class ZON;
};

class MapBlock;
class IIndexBuffer;

class Map : public Entity {
public:
	Map();
	virtual ~Map();

	void Load(const char* zon);

	virtual void Update();
	virtual int UpdateCulling(const Frustum& camf);

	virtual void Render();

	void AddObjectsToScene(class SceneManager* scene);

	bool GetHeight(Vector3& out) const;

	bool CastRay(const Ray& ray, Vector3& out) const;
	bool CastRayDown(const Ray& ray, Vector3& out) const;

private:
	ROSE::ZON* mZoneData;

	Range<int> mBlocksX;
	Range<int> mBlocksY;
	MapBlock* mBlocks[64][64];
	IIndexBuffer* mIndexBuffer;
};

#endif
