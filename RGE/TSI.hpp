#ifndef TSI_HPP
#define TSI_HPP

#include "File.hpp"
#include "ScopedPointer.hpp"
#include "Array.hpp"
#include "Rect.hpp"
#include "StringMap.hpp"
#include "IRender.hpp"

namespace ROSE {
	class TSI {
	public:
		struct Graphic {
			short mOwner;
			Rect mRect;
			char mName[0x20];
			IImage* mImage;
			
			void Render(){
				gUiRender->DrawImage(mImage, mRect.mMin.x, mRect.mMin.y, mRect.mMax.x, mRect.mMax.y);
			}
			
			void Render(int width, int height){
				if(width == 0) width = mRect.mMax.x - mRect.mMin.x;
				if(height == 0) height = mRect.mMax.y - mRect.mMin.y;
				gUiRender->DrawImage(mImage, mRect.mMin.x, mRect.mMin.y, mRect.mMax.x, mRect.mMax.y, width, height);
			}
		};

	public:
		TSI();
		~TSI();

		bool Load(const char* path);

		void AddToMap(StringMap<Graphic*>& map);

	private:
		Array<IImage*> mImages;
		Array<Graphic> mGraphics;
	};
};

#endif
