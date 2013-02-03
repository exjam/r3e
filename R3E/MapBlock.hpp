#ifndef MAP_BLOCK_H
#define MAP_BLOCK_H

#include "Ray.hpp"
#include "Range.hpp"
#include "Entity.hpp"
#include "Texture.hpp"
#include "QuadTree.hpp"
#include "SmartPointer.hpp"

namespace ROSE {
	class IFO;
	class TIL;
	class HIM;
	class LIT;
	class ZON;
};

class IIndexBuffer;
class IVertexBuffer;

class MapBlock : public Entity {
public:
	MapBlock(IIndexBuffer* iBuffer);
	virtual ~MapBlock();

	virtual void Render();
	virtual void Update();
	virtual int UpdateCulling(const Frustum& camf);

	void SetMapInfo(const char* path);
	void SetTilemap(const char* path);
	void SetLightmap(const char* path);
	void SetHeightmap(const char* path);
	void SetObjectLit(const char* path);
	void SetBuildingLit(const char* path);

	void AddObjectsToScene(class SceneManager* scene);

	void GenerateTerrain(ROSE::ZON* zon);

	float GetHeight(float x, float y) const;

	void CastRay(const Ray& ray, float& tMin) const;
	bool CastRayDown(const Ray& ray, float& tMin) const;

private:
	bool GetPatchIntersect(int index, const Ray& ray, float& tMin) const;
	void QuadIntersectRay(const QuadTree::Node* node, const Ray& ray, float& tMin) const;

private:
	QuadTree mQuadTree;
	bool mPatchVisible[16*16];

	IIndexBuffer* mIndexBuffer;
	IVertexBuffer* mVertexBuffer;

	ROSE::IFO* mMapInfo;
	ROSE::TIL* mTilemap;
	ROSE::HIM* mHeightmap;
	ROSE::LIT* mBuildingLit;
	ROSE::LIT* mObjectLit;

	SmartPointer<Texture> mLightmap;
	GLuint mLayer1[16*16];
	GLuint mLayer2[16*16];
};

#endif