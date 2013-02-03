#ifndef LIT_HPP
#define LIT_HPP

#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "Matrix4.hpp"
#include "Array.hpp"

namespace ROSE {
	class LIT {
	public:
		struct Object {
			struct Part {
				Part(){}
				~Part(){}

				unsigned int mIndex;
				Matrix4 mTransform;
				SmartPointer<Texture> mTexture;
				float xPos, yPos, scale;
			};

			Object(){}
			~Object(){}

			unsigned int mIndex;
			Array<Part> mParts;
		};

	public:
		LIT();
		~LIT();

		bool Load(const char* path);

	public:
		Array<Object> mObjectList;
	};
};

#endif
