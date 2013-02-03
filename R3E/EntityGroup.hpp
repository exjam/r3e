#ifndef ENTITY_GROUP_H
#define ENTITY_GROUP_H

#include "Entity.hpp"
#include "Array.hpp"

class EntityGroup : public Entity {
public:
	EntityGroup();
	virtual ~EntityGroup();

	virtual void Render();

	virtual void UpdateBoundingBox();
	virtual void UpdateAnimation(Matrix4* mat);
	virtual int UpdateCulling(const Frustum& camf);

	virtual void AddChild(Entity* child);
	virtual Entity* RemoveChild(Entity* child);

	virtual bool IsGroup();
	void SetGroupType(int type);

public:
	Array<Entity*> mChildren;
};


#endif