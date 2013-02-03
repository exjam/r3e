#ifndef ZMD_H
#define ZMD_H

#include "String.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Matrix4.hpp"
#include "Array.hpp"
#include "Resource.hpp"

namespace ROSE {
	class ZMD : public Resource {
	public:
		struct Bone {
			bool mDummy;
			unsigned int mParentID;
			String mName;

			Vector3 mTranslate;
			Quaternion mRotate;

			Matrix4 mTransform;
		};

	public:
		ZMD();
		~ZMD();

		bool Load(const char* path);
		void Unload();

		void Render();

		void TransformChildren(unsigned int parent);

		Bone* GetBone(int id) const;
		Bone* GetBone(const char* name) const;
		unsigned int GetBoneID(const char* name) const;

		unsigned int GetBoneCount() const;
		const Array<Bone>& GetBoneList() const;

	public:
		unsigned int mVersion;
		Array<Bone> mBoneList;
	};
};

#endif