#include "Global.h"

#include "Map.hpp"
#include "ZON.hpp"
#include "String.hpp"
#include "MapBlock.hpp"
#include "SafeDelete.hpp"
#include "IndexBuffer.hpp"

Map::Map() : mIndexBuffer(0), mZoneData(0) {
	mType = ENTITY_3TEX_TERRAIN;
	memset(mBlocks, 0, sizeof(MapBlock*) * 64 * 64);
}

Map::~Map(){
	for(int x = 0; x < 64; ++x){
		for(int y = 0; y < 64; ++y){
			SAFE_DELETE(mBlocks[x][y]);
		}
	}

	SAFE_DELETE(mZoneData);
	SAFE_DELETE(mIndexBuffer);
}

void Map::Load(const char* zon){
	String directory = zon;
	directory.Null((int)(strrchr(zon, '\\') - zon));

	mBlocksX = Range<int>(64, 0);
	mBlocksY = Range<int>(64, 0);

	short predefindices[] = { 
		 0,  5,  1,  6,  2,  7,  3,  8,  4,  9,    9,  5,
		 5, 10,  6, 11,  7, 12,  8, 13,  9, 14,   14, 10,
		10, 15, 11, 16, 12, 17, 13, 18, 14, 19,   19, 15,
		15, 20, 16, 21, 17, 22, 18, 23, 19, 24
	};

	mIndexBuffer = new IndexBuffer<short>();
	mIndexBuffer->SetCount(46);
	memcpy(mIndexBuffer->Buffer(), predefindices, sizeof(short) * 46);
	mIndexBuffer->SetType(GL_TRIANGLE_STRIP);

	mZoneData = new ROSE::ZON();
	mZoneData->Load(zon);

	BoundingBox bbox;
	String path;
	for(int x = 0; x < 64; ++x){
		for(int y = 0; y < 64; ++y){
			path = String("%1\\%2_%3.HIM").arg(directory.Str()).arg(x).arg(y);
			if(!FILE_SYS()->FileExists(path)) continue;
			mBlocksX.AddValue(x);
			mBlocksY.AddValue(y);

			MapBlock* block = new MapBlock(mIndexBuffer);
			mBlocks[x][y] = block;

			block->SetTransform(Matrix4::CreateTranslation(Vector3(160.0f * x, 160.0f * (65-y), 0.0f)));
			block->SetHeightmap(path);

			path.Replace(path.Length() - 3, 3, "TIL");
			block->SetTilemap(path);

			path.Replace(path.Length() - 3, 3, "IFO");
			block->SetMapInfo(path);

			path = String("%1\\%2_%3\\%2_%3_PLANELIGHTINGMAP.DDS").arg(directory.Str()).arg(x).arg(y);
			block->SetLightmap(path);

			path = String("%1\\%2_%3\\LIGHTMAP\\OBJECTLIGHTMAPDATA.LIT").arg(directory.Str()).arg(x).arg(y);
			block->SetObjectLit(path);

			path = String("%1\\%2_%3\\LIGHTMAP\\BUILDINGLIGHTMAPDATA.LIT").arg(directory.Str()).arg(x).arg(y);
			block->SetBuildingLit(path);

			block->GenerateTerrain(mZoneData);

			bbox.AddBox(block->GetBoundingBox());
		}
	}

	SetBoundingBox(bbox);
}

void Map::AddObjectsToScene(class SceneManager* scene){
	for(int x = mBlocksX.mMin; x <= mBlocksX.mMax; ++x){
		for(int y = mBlocksY.mMin; y <= mBlocksY.mMax; ++y){
			if(!mBlocks[x][y]) continue;
			mBlocks[x][y]->AddObjectsToScene(scene);
		}
	}
}

void Map::Update(){}

int Map::UpdateCulling(const Frustum& camf){
	int ret = mBoundingBoxTransformed.CheckIntersectFrustum(camf);
	mVisible = (ret != 0);

	for(int x = mBlocksX.mMin; x <= mBlocksX.mMax; ++x){
		for(int y = mBlocksY.mMin; y <= mBlocksY.mMax; ++y){
			if(!mBlocks[x][y]) continue;
			mBlocks[x][y]->UpdateCulling(camf);
		}
	}

	return ret;
}

void Map::Render(){
	mIndexBuffer->Bind();
	for(int x = mBlocksX.mMin; x <= mBlocksX.mMax; ++x){
		for(int y = mBlocksY.mMin; y <= mBlocksY.mMax; ++y){
			if(!mBlocks[x][y]) continue;
			mBlocks[x][y]->Render();
		}
	}
}

bool Map::GetHeight(Vector3& out) const {
	int bX = int(out.x) / 160;
	int bY = 64 - (int(out.y) / 160);
	if(bX < 0 || bX >= 64 || bY < 0 || bY >= 64 || !mBlocks[bX][bY]) return false;
	out.z = mBlocks[bX][bY]->GetHeight(out.x, out.y);
	return true;
}

bool Map::CastRay(const Ray& ray, Vector3& out) const {
	float tMin = 9999.0f;
	int initVal = *(int*)&tMin;
	for(int x = mBlocksX.mMin; x <= mBlocksX.mMax; ++x){
		for(int y = mBlocksY.mMin; y <= mBlocksY.mMax; ++y){
			if(!mBlocks[x][y]) continue;
			mBlocks[x][y]->CastRay(ray, tMin);
		}
	}

	if(initVal == *(int*)&tMin) return false;
	out = ray.GetPosition(tMin);
	return true;
}

bool Map::CastRayDown(const Ray& ray, Vector3& out) const {
	int bX = int(ray.mPoint.x) / 160;
	int bY = 64 - (int(ray.mPoint.y) / 160);
	if(bX < 0 || bX >= 64 || bY < 0 || bY >= 64 || !mBlocks[bX][bY]) return false;
	
	float tval;
	if(!mBlocks[bX][bY]->CastRayDown(ray, tval)) return false;
	out = ray.GetPosition(tval);
	return true;
}
