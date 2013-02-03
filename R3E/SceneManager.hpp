#ifndef SCENE_MAN_H
#define SCENE_MAN_H

#include "Array.hpp"
#include "Point.hpp"
#include "Entity.hpp"
#include "SafeDelete.hpp"

enum MouseFlags {
	MOUSE_NO_FLAGS	= 0,
	MOUSE_LBUTTON	= 1 << 0,
	MOUSE_RBUTTON	= 1 << 1,
	MOUSE_SHIFT		= 1 << 2,
	MOUSE_CONTROL	= 1 << 3,
	MOUSE_MBUTTON	= 1 << 4,
	MAX_MOUSE_FLAGS,
};

class Map;
class SkyBox;
class Entity;
class Camera;
class ShaderPair;

class SceneManager {
private:
public:
	SceneManager();
	~SceneManager();

	void Init();

	void SetMap(Map* map);
	void SetSkyBox(SkyBox* box);
	void SetCulling(bool enabled);
	void SetCamera(Camera* camera);

	Entity* GetHoverEntity();
	Entity* AddEntity(Entity* entity);
	Entity* RemoveEntity(Entity* entity);

	void ClearScene();

	void BeginScene();
	void EndScene();

	void UpdateScene();
	void RenderScene();
	void ResizeScene(int width, int height);

	void Begin2D();
	void End2D();

	void UpdateViewport();
	void MouseMove(int x, int y, int flags);

	unsigned int EntityCount() const;

	Vector3 GetScreenPosition(Vector3& world) const;
	Entity* CastRayDown(const Ray& ray, Vector3& out) const;
	Entity* CastRay(const Ray& ray, Vector3& out, float maxLengthSq) const;

private:
	bool mSizeChanged;
	bool mCulling;
	Camera* mCamera;
	Entity* mHoverEntity;
	Map* mMap;
	SkyBox* mSkyBox;

	Array<Entity*, 10> mEntityList[MAX_ENTITY];
	Array<Entity*, 10> mEntityFlag[ENTITY_FLAG_COUNT];
	Array<Entity*, 10> mAllEntities;

	int mWidth, mHeight;

	Array<ShaderPair*> mShaders;
};

#endif