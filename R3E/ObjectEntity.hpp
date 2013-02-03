#ifndef OBJECT_ENTITY_HPP
#define OBJECT_ENTITY_HPP

#include "LIT.hpp"
#include "Entity.hpp"
#include "Texture.hpp"
#include "SmartPointer.hpp"

class EntityGroup;

class ObjectEntity : public Entity {
public:
	ObjectEntity(int objtype, int objid);
	virtual ~ObjectEntity();

	virtual void UpdateBoundingBox();

	virtual void Render();
	virtual bool IsSkinned();

	virtual bool CastRay(const Ray& ray, Vector3& out, float maxLengthSq);
	virtual bool CastRayDown(const Ray& ray, Vector3& out);

	void SetMesh(EntityGroup* mesh);
	void SetLitObject(ROSE::LIT::Object* obj);

	int GetObjectID() const;
	int GetObjectType() const;

private:
	int mObjectType;
	int mObjectID;
	EntityGroup* mMeshData;
	ROSE::LIT::Object* mLitObject;
	SmartPointer<Texture> mLightmap;
};

#endif
