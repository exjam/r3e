#ifndef MESH_1TEX_ENTITY
#define MESH_1TEX_ENTITY

#include "ZMS.hpp"
#include "Entity.hpp"
#include "Material.hpp"
#include "ResourceManager.hpp"
#include "Ray.hpp"

class Mesh1TexEntity : public Entity {
public:
	Mesh1TexEntity() : mVB(0) {
		mType = ENTITY_1TEX_MESH;
	}

	virtual ~Mesh1TexEntity(){
		SAFE_DELETE(mVB);
	}

	virtual void Render(){
		if(mHasTransform){
			glPushMatrix();
			mTransform.glMult();
		}

		mMaterial.Apply();
		if(!IsSkinned()) mMesh->Render();
		else if(mVB){
			mVB->Bind();
			mMesh->RenderNoVB();
		}

		if(mHasTransform)
			glPopMatrix();
	}

	virtual bool CastRay(const Ray& ray, Vector3& out, float maxLengthSq){
		if(!ray.IntersectsBox(mBoundingBoxTransformed)) return false;

		Ray localRay;
		Matrix4 invs = Matrix4::CreateInverse(mTransform);
		localRay.mPoint = invs.TransformCoord(ray.mPoint);
		localRay.mDirection = invs.TransformNormal(ray.mDirection);
		if(!mMesh->CastRay(localRay, out)) return false;

		out = mTransform.TransformCoord(out);
		return true;
	}

	virtual bool CastRayDown(const Ray& ray, Vector3& out){
		if(!mBoundingBoxTransformed.Contains2D(ray.mPoint)) return false;

		Ray localRay;
		Matrix4 invs = Matrix4::CreateInverse(mTransform);
		localRay.mPoint = invs.TransformCoord(ray.mPoint);
		localRay.mDirection = invs.TransformNormal(ray.mDirection);
		if(!mMesh->CastRay(localRay, out)) return false;

		out = mTransform.TransformCoord(out);
		return true;
	}

	virtual bool IsSkinned(){
		return mMesh->mFormat.HasSkin();
	}

	virtual void UpdateAnimation(Matrix4* mat){
		if(!mVB)
			mVB = mMesh->CreateVertexBuffer();

		mMesh->UpdateAnimation(mat, mVB);
	}

	Mesh1TexEntity* SetMesh(const char* path){
		mMesh = MESH_MGR().Load(path);
		SetBoundingBox(mMesh->mBoundingBox);
		return this;
	}

	Mesh1TexEntity* SetMaterial(Material& mat){
		mMaterial = mat;
		return this;
	}

private:
	IVertexBuffer* mVB;
	SmartPointer<ROSE::ZMS> mMesh;
	Material mMaterial;
};

#endif