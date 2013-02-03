#include "Global.h"

#include "ZMS.hpp"
#include "Log.hpp"
#include "FileSystem.hpp"
#include "ScopedPointer.hpp"

namespace ROSE {
	ZMS::ZMS() : mVersion(0), mBoneLookup(0), mVertices(0), mNormals(0), mWeights(0),
		mColours(0), mBones(0), mTangents(0), mIndexBuffer(0), mVertexBuffer(0)
	{
		mUVMaps[0] = 0;
		mUVMaps[1] = 0;
		mUVMaps[2] = 0;
		mUVMaps[3] = 0;
	}

	ZMS::~ZMS(){}

	void ZMS::Render(){
		mVertexBuffer->Bind();
		mIndexBuffer->Bind();
		mIndexBuffer->Draw();
	}

	void ZMS::RenderNoVB(){
		mIndexBuffer->Bind();
		mIndexBuffer->Draw();
	}

	bool ZMS::CastRay(const Ray& ray, Vector3& out){
		float tMin = 9999.0f;
		int initVal = *(int*)&tMin;
		short* indices = (short*)mIndexBuffer->Buffer();

		for(short i = 0; i < mIndexCount; i += 3){
			Vector3& v0 = mVertices[indices[i]];
			Vector3& v1 = mVertices[indices[i + 1]];
			Vector3& v2 = mVertices[indices[i + 2]];
			
			float tVal;
			if(ray.IntersectTriangle(v0, v1, v2, tVal))
				if(tVal < tMin)
					tMin = tVal;
		}

		if(initVal == *(int*)&tMin) return false;
		out = ray.GetPosition(tMin);
		return true;
	}

	IVertexBuffer* ZMS::CreateVertexBuffer(){
		if(!mFormat.HasSkin()) return false;

		VertexBuffer<V3F_UV0>* srcbuf = (VertexBuffer<V3F_UV0>*)mVertexBuffer;
		VertexBuffer<V3F_UV0>* newbuf = new VertexBuffer<V3F_UV0>();
		newbuf->SetCount(srcbuf->Size());
		newbuf->SetDynamic(true);
	
		memcpy(newbuf->mVertices.Data(), srcbuf->mVertices.Data(), srcbuf->mVertices.Size() * sizeof(V3F_UV0));

		return newbuf;
	}

	void ZMS::UpdateAnimation(Matrix4* mat, IVertexBuffer* buf){
		if(!mFormat.HasSkin() || !mat) return;
		VertexBuffer<V3F_UV0>* vbuf = (VertexBuffer<V3F_UV0>*)buf;
		for(unsigned int i = 0; i < vbuf->mVertices.Size(); ++i){
			Vector3 pos;
			pos += mat[mBoneLookup[int(mBones[i].x)]].TransformCoord(mVertices[i]) * mWeights[i].x;
			pos += mat[mBoneLookup[int(mBones[i].y)]].TransformCoord(mVertices[i]) * mWeights[i].y;
			pos += mat[mBoneLookup[int(mBones[i].z)]].TransformCoord(mVertices[i]) * mWeights[i].z;
			pos += mat[mBoneLookup[int(mBones[i].w)]].TransformCoord(mVertices[i]) * mWeights[i].w;
			vbuf->mVertices[i].pos = pos;
		}

		vbuf->UpdateBuffer();
	}

	void ZMS::Unload(){
		SAFE_DELETE_ARRAY(mBoneLookup);
		SAFE_DELETE_ARRAY(mVertices);
		SAFE_DELETE_ARRAY(mNormals);
		SAFE_DELETE_ARRAY(mColours);
		SAFE_DELETE_ARRAY(mBones);
		SAFE_DELETE_ARRAY(mWeights);
		SAFE_DELETE_ARRAY(mTangents);
		SAFE_DELETE_ARRAY(mUVMaps[0]);
		SAFE_DELETE_ARRAY(mUVMaps[1]);
		SAFE_DELETE_ARRAY(mUVMaps[2]);
		SAFE_DELETE_ARRAY(mUVMaps[3]);
		SAFE_DELETE(mVertexBuffer);
		SAFE_DELETE(mIndexBuffer);
	}

	bool ZMS::Load(const char* path){
		ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
		if(!fh) return false;

		char VersionHeader[8];
		fh->ReadData(VersionHeader, 8);
		if(strcmp(VersionHeader, "ZMS0008")){
			mVersion = 8;
		}else if(strcmp(VersionHeader, "ZMS0007")){
			mVersion = 7;
		}else if(strcmp(VersionHeader, "ZMS0006")){
			mVersion = 6;
		}else if(strcmp(VersionHeader, "ZMS0005")){
			mVersion = 5;
		}else{
			LOG("Error: '%s' invalid version '%s'", path, VersionHeader);
			fh->Close();
			return false;
		}

		if(mVersion >= 7){
			LoadMesh8(fh);
		}else{
			LoadMesh6(fh);
		}

		static int highestBCount = 0;
		if(highestBCount < mBoneCount) highestBCount = mBoneCount;

		mBoundingBox.mMax = Vector3(-99999.0f);
		mBoundingBox.mMin = Vector3( 99999.0f);

		if(!mFormat.HasUV0()){
			mVertexBuffer = new VertexBuffer<V3F>();
			mVertexBuffer->SetCount(mVertexCount);
			VertexBuffer<V3F>* vbuf = (VertexBuffer<V3F>*)mVertexBuffer;
			for(int i = 0; i < mVertexCount; ++i){
				vbuf->mVertices[i] = mVertices[i];
				mBoundingBox.AddPoint(mVertices[i]);
			}
		}else if(!mFormat.HasUV1()){
			V3F_UV0 tmp;
			mVertexBuffer = new VertexBuffer<V3F_UV0>();
			mVertexBuffer->SetCount(mVertexCount);
			VertexBuffer<V3F_UV0>* vbuf = (VertexBuffer<V3F_UV0>*)mVertexBuffer;
			for(int i = 0; i < mVertexCount; ++i){
				tmp.pos = mVertices[i];
				tmp.uv0 = mUVMaps[0][i];
				vbuf->mVertices[i] = tmp;
				mBoundingBox.AddPoint(mVertices[i]);
			}
		}else{
			V3F_UV0_UV1 tmp;
			mVertexBuffer = new VertexBuffer<V3F_UV0_UV1>();
			mVertexBuffer->SetCount(mVertexCount);
			VertexBuffer<V3F_UV0_UV1>* vbuf = (VertexBuffer<V3F_UV0_UV1>*)mVertexBuffer;
			for(int i = 0; i < mVertexCount; ++i){
				tmp.pos = mVertices[i];
				tmp.uv0 = mUVMaps[0][i];
				tmp.uv1 = mUVMaps[1][i];
				vbuf->mVertices[i] = tmp;
				mBoundingBox.AddPoint(mVertices[i]);
			}
		}

		if(mFormat.HasSkin())
			mVertexBuffer->SetDynamic(true);

		LOG("Loaded: '%s' V: %u I: %u", path, mVertexBuffer->Size(), mIndexBuffer->Size());

		fh->Close();
		return true;
	}

	void ZMS::LoadMesh6(File* fh){
		int idx;
		fh->Read(mFormat);
		fh->Read(mBoundingBox.mMin);
		fh->Read(mBoundingBox.mMax);

		mBoneCount = (short)fh->Read<int>();
		mBoneLookup = new short[mBoneCount];
		for(int i = 0; i < mBoneCount; ++i){
			fh->Read(idx);
			mBoneLookup[i] = (short)fh->Read<int>();
		}

		mVertexCount = (short)fh->Read<int>();
		mVertices = new Vector3[mVertexCount];
		for(int i = 0; i < mVertexCount; ++i){
			fh->Read(idx);
			fh->Read(mVertices[i]);
		}

		if(mFormat.HasNormal()){
			mNormals = new Vector3[mVertexCount];
			for(int i = 0; i < mVertexCount; ++i){
				fh->Read(idx);
				fh->Read(mNormals[i]);
			}
		}

		if(mFormat.HasColour()){
			mColours = new Vector4[mVertexCount];
			for(int i = 0; i < mVertexCount; ++i){
				fh->Read(idx);
				fh->Read(mColours[i]);
			}
		}

		if(mFormat.HasSkin()){
			mWeights = new Vector4[mVertexCount];
			mBones = new Vector4[mVertexCount];
			int bones[4];
			for(int i = 0; i < mVertexCount; ++i){
				fh->Read(idx);

				fh->Read(mWeights[i]);
				fh->ReadData(bones, sizeof(int) * 4);
				mBones[i] = Vector4((float)bones[0], (float)bones[1], (float)bones[2], (float)bones[3]);
			}
		}

		if(mFormat.HasTangents()){
			mTangents = new Vector3[mVertexCount];
			for(int i = 0; i < mVertexCount; ++i){
				fh->Read(idx);
				fh->Read(mTangents[i]);
			}
		}

		for(int c = 0; c < 4; ++c){
			if(!mFormat.HasUV(c)) continue;
			mUVMaps[c] = new Vector2[mVertexCount];
			for(int i = 0; i < mVertexCount; ++i){
				fh->Read(idx);
				fh->Read(mUVMaps[c][i]);
			}
		}

		short faceCount = (short)fh->Read<int>();
		mIndexCount = faceCount * 3;

		mIndexBuffer = new IndexBuffer<short>();
		mIndexBuffer->SetSize(mIndexCount);

		short tmp;
		for(int i = 0; i < faceCount; ++i){
			fh->Read(idx);

			for(int j = 0; j < 3; ++j){
				tmp = (short)fh->Read<int>();
				mIndexBuffer->AddIndex(&tmp);
			}
		}
	}

	void ZMS::LoadMesh8(File* fh){
		fh->Read(mFormat);
		fh->Read(mBoundingBox.mMin);
		fh->Read(mBoundingBox.mMax);

		fh->Read(mBoneCount);
		mBoneLookup = new short[mBoneCount];
		fh->ReadData(mBoneLookup, sizeof(short) * mBoneCount);

		fh->Read(mVertexCount);
		mVertices = new Vector3[mVertexCount];
		fh->ReadData(mVertices, sizeof(Vector3) * mVertexCount);

		if(mFormat.HasNormal()){
			mNormals = new Vector3[mVertexCount];
			fh->ReadData(mNormals, sizeof(Vector3) * mVertexCount);
		}

		if(mFormat.HasColour()){
			mColours = new Vector4[mVertexCount];
			fh->ReadData(mColours, sizeof(Vector4) * mVertexCount);
		}

		if(mFormat.HasSkin()){
			mWeights = new Vector4[mVertexCount];
			mBones = new Vector4[mVertexCount];
			short bones[4];
			for(int i = 0; i < mVertexCount; ++i){
				fh->Read(mWeights[i]);
				fh->ReadData(bones, sizeof(short) * 4);
				mBones[i] = Vector4((float)bones[0], (float)bones[1], (float)bones[2], (float)bones[3]);
			}
		}

		if(mFormat.HasTangents()){
			mTangents = new Vector3[mVertexCount];
			fh->ReadData(mTangents, sizeof(Vector3) * mVertexCount);
		}

		for(int c = 0; c < 4; ++c){
			if(!mFormat.HasUV(c)) continue;
			mUVMaps[c] = new Vector2[mVertexCount];
			fh->ReadData(mUVMaps[c], sizeof(Vector2) * mVertexCount);
		}

		fh->Read(mIndexCount);

		mIndexCount *= 3;

		mIndexBuffer = new IndexBuffer<short>();
		mIndexBuffer->SetCount(mIndexCount);
		fh->ReadData(mIndexBuffer->Buffer(), sizeof(short) * mIndexCount);
	}
};
