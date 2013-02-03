#ifndef ZMO_H
#define ZMO_H

#include "String.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Matrix4.hpp"
#include "Array.hpp"
#include "SafeDelete.hpp"
#include "ZMD.hpp"
#include "Resource.hpp"

namespace ROSE {
	class ZMO : public Resource {
	private:
		enum ChannelTypes {
			CTYPE_NONE = (1 << 0),
			CTYPE_POSITION = (1 << 1),
			CTYPE_ROTATION = (1 << 2),
			CTYPE_NORMAL = (1 << 3),
			CTYPE_ALPHA = (1 << 4),
			CTYPE_UV0 = (1 << 5),
			CTYPE_UV1 = (1 << 6),
			CTYPE_UV2 = (1 << 7),
			CTYPE_UV3 = (1 << 8),
			CTYPE_TEXTUREANIM = (1 << 9),
			CTYPE_SCALE = (1 << 10),
		};

	public:
		class Channel {
		public:
			Channel() : mData(0) {}
			~Channel(){
				SAFE_DELETE_ARRAY(mData);
			}

			int mType;
			int mBoneID;
			char* mData;
		};

	public:
		ZMO();
		~ZMO();

		bool Load(const char* path);
		void Unload();

		unsigned int GetFPS() const;
		unsigned int GetFrameCount() const;

		Channel* GetChannel(int idx) const;
		Channel* GetBoneChannel(int bone, int type) const;

		void CreateCameraMatrices(Array<Matrix4>& matrices) const;
		void CreateBoneMatrices(int frame, const Array<ZMD::Bone>& bindBoneList, Array<Matrix4>& matrices) const;

	private:
		void TransformChildren(const Array<ZMD::Bone>& bindBoneList, Array<Matrix4>& matrices, unsigned int parent) const;

	public:
		unsigned int mFPS;
		unsigned int mFrameCount;
		Array<Channel> mChannels;
	};
};

#endif