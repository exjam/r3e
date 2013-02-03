#ifndef ROSE_ZMS_H
#define ROSE_ZMS_H

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "BoundingBox.hpp"

#include "VertexTypes.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

#include "Resource.hpp"

#include "Ray.hpp"

namespace ROSE {
	class ZMS : public Resource {
	private:
		struct BoneWeights {
			float mWeights[4];
			short mBones[4];
		};

		struct VertexFormat {
			enum {
				VF_NONE = (1 << 0),
				VF_POSITION = (1 << 1),
				VF_NORMAL = (1 << 2),
				VF_COLOR = (1 << 3),
				VF_BLEND_WEIGHT = (1 << 4),
				VF_BLEND_INDEX = (1 << 5),
				VF_TANGENT = (1 << 6),
				VF_UV0 = (1 << 7),
				VF_UV1 = (1 << 8),
				VF_UV2 = (1 << 9),
				VF_UV3 = (1 << 10),
			};

			bool HasPosition(){
				return (mFormat & VF_POSITION) != 0;
			}

			bool HasNormal(){
				return (mFormat & VF_NORMAL) != 0;
			}

			bool HasColour(){
				return (mFormat & VF_COLOR) != 0;
			}

			bool HasBoneWeight(){
				return (mFormat & VF_BLEND_WEIGHT) != 0;
			}

			bool HasBoneIndex(){
				return (mFormat & VF_BLEND_INDEX) != 0;
			}

			bool HasSkin(){
				return HasBoneWeight() && HasBoneIndex();
			}

			bool HasTangents(){
				return (mFormat & VF_TANGENT) != 0;
			}

			bool HasUV(int channel){
				return (mFormat & (VF_UV0 << channel)) != 0;
			}

			bool HasUV0(){
				return (mFormat & VF_UV0) != 0;
			}

			bool HasUV1(){
				return (mFormat & VF_UV1) != 0;
			}

			bool HasUV2(){
				return (mFormat & VF_UV2) != 0;
			}

			bool HasUV3(){
				return (mFormat & VF_UV3) != 0;
			}

			int UVCount(){
				if(HasUV3()) return 4;
				if(HasUV2()) return 3;
				if(HasUV1()) return 2;
				if(HasUV0()) return 1;
				return 0;
			}

			int mFormat;
		};

	public:
		ZMS();
		~ZMS();

		void Render();
		void RenderNoVB();

		IVertexBuffer* CreateVertexBuffer();
		void UpdateAnimation(Matrix4* mat, IVertexBuffer* buf);

		
		bool Load(const char* path);
		void Unload();

		bool CastRay(const Ray& ray, Vector3& out);

	private:
		void LoadMesh6(File* fh);
		void LoadMesh8(File* fh);

	public:
		VertexFormat mFormat;
		BoundingBox mBoundingBox;

	private:
		int mVersion;

		short mBoneCount;
		short* mBoneLookup;

		short mVertexCount;
		Vector3* mVertices;
		Vector3* mNormals;
		Vector4* mColours;
		Vector4* mBones;
		Vector4* mWeights;
		Vector3* mTangents;
		Vector2* mUVMaps[4];

		short mIndexCount;

		IVertexBuffer* mVertexBuffer;
		IIndexBuffer* mIndexBuffer;
	};
};

#endif