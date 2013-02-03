#ifndef PARTICLE_H
#define PARTICLE_H

#include "FileSystem.hpp"
#include "String.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Matrix4.hpp"
#include "Range.hpp"
#include "Array.hpp"

namespace ROSE {
	class PTL {
	private:
		enum ChannelType {
			CHAN_NONE = 0,
			CHAN_SIZE = 1,
			CHAN_TIMER = 2,
			CHAN_REDCOLOR = 3,
			CHAN_GREENCOLOR = 4,
			CHAN_BLUECOLOR = 5,
			CHAN_ALPHA = 6,
			CHAN_COLOR = 7,
			CHAN_VELOCITYX = 8,
			CHAN_VELOCITYY = 9,
			CHAN_VELOCITYZ = 10,
			CHAN_VELOCITY = 11,
			CHAN_TEXTURE = 12,
			CHAN_ROTATION = 13,
		};

		class IKeyFrame {
		public:
			IKeyFrame() : mNext(0) {}
			virtual ~IKeyFrame(){}

			virtual void Load(File* fh){
				fh->Read(mStartTime);
				mFade = (fh->Read<unsigned char>() != 0);
			}

			unsigned int mChannelType;
			Range<float> mStartTime;
			bool mFade;

			IKeyFrame* mNext;

			float mActualStartTime;
		};

		class KFSize : public IKeyFrame {
		public:
			KFSize(){
				mChannelType = CHAN_SIZE;
			}

			virtual ~KFSize(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mSize);
			}

			Range<Vector2> mSize;
		};

		class KFTimer : public IKeyFrame {
		public:
			KFTimer(){
				mChannelType = CHAN_TIMER;
			}

			virtual ~KFTimer(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mTimer);
			}

			Range<float> mTimer;
		};

		class KFRed : public IKeyFrame {
		public:
			KFRed(){
				mChannelType = CHAN_REDCOLOR;
			}

			virtual ~KFRed(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mRed);
			}

			Range<float> mRed;
		};

		class KFGreen : public IKeyFrame {
		public:
			KFGreen(){
				mChannelType = CHAN_GREENCOLOR;
			}

			virtual ~KFGreen(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mGreen);
			}

			Range<float> mGreen;
		};

		class KFBlue : public IKeyFrame {
		public:
			KFBlue(){
				mChannelType = CHAN_BLUECOLOR;
			}

			virtual ~KFBlue(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mBlue);
			}

			Range<float> mBlue;
		};

		class KFAlpha : public IKeyFrame {
		public:
			KFAlpha(){
				mChannelType = CHAN_ALPHA;
			}

			virtual ~KFAlpha(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mAlpha);
			}

			Range<float> mAlpha;
		};

		class KFColour : public IKeyFrame {
		public:
			KFColour(){
				mChannelType = CHAN_COLOR;
			}

			virtual ~KFColour(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mColour);
			}

			Range<Vector4> mColour;
		};

		class KFVelocityX : public IKeyFrame {
		public:
			KFVelocityX(){
				mChannelType = CHAN_VELOCITYX;
			}

			virtual ~KFVelocityX(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mVelocityX);
			}

			Range<float> mVelocityX;
		};

		class KFVelocityY : public IKeyFrame {
		public:
			KFVelocityY(){
				mChannelType = CHAN_VELOCITYY;
			}

			virtual ~KFVelocityY(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mVelocityY);
			}

			Range<float> mVelocityY;
		};

		class KFVelocityZ : public IKeyFrame {
		public:
			KFVelocityZ(){
				mChannelType = CHAN_VELOCITYZ;
			}

			virtual ~KFVelocityZ(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mVelocityZ);
			}

			Range<float> mVelocityZ;
		};

		class KFVelocity : public IKeyFrame {
		public:
			KFVelocity(){
				mChannelType = CHAN_VELOCITY;
			}

			virtual ~KFVelocity(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mVelocity);
			}

			Range<Vector3> mVelocity;
		};

		class KFTexture : public IKeyFrame {
		public:
			KFTexture(){
				mChannelType = CHAN_TEXTURE;
			}

			virtual ~KFTexture(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mTexture);
			}

			Range<float> mTexture;
		};

		class KFRotation : public IKeyFrame {
		public:
			KFRotation(){
				mChannelType = CHAN_ROTATION;
			}

			virtual ~KFRotation(){}
			
			virtual void Load(File* fh){
				IKeyFrame::Load(fh);
				fh->Read(mRotation);
			}

			Range<float> mRotation;
		};

		class ParticleEmitter {
		public:
			ParticleEmitter(){}
			~ParticleEmitter(){
				mKeyFrames.DeleteValues();
			}

			String mName;
			Range<float> mLifetime;
			Range<float> mEmitRate;
			unsigned int mLoopCount;
			Range<Vector3> mSpawnDirection;
			Range<Vector3> mEmitRadius;
			Range<Vector3> mGravity;
			String mTexture;
			unsigned int mParticleCount;
			unsigned int mAlignType;
			unsigned int mUpdateCoordinates;
			unsigned int mTextureWidth;
			unsigned int mTextureHeight;
			unsigned int mSpriteType;
			unsigned int mBlendDest;
			unsigned int mBlendSrc;
			unsigned int mBlendOp;

			Array<IKeyFrame*> mKeyFrames;
		};

	public:
		PTL(){}
		~PTL(){
			mEmitters.Clear();
		}

		IKeyFrame* CreateKeyFrame(File* fh){
			unsigned int type;
			fh->Read(type);
			switch(type){
				case CHAN_SIZE:
					return new KFSize();
				case CHAN_TIMER:
					return new KFTimer();
				case CHAN_REDCOLOR:
					return new KFRed();
				case CHAN_GREENCOLOR:
					return new KFGreen();
				case CHAN_BLUECOLOR:
					return new KFBlue();
				case CHAN_ALPHA:
					return new KFAlpha();
				case CHAN_COLOR:
					return new KFColour();
				case CHAN_VELOCITYX:
					return new KFVelocityX();
				case CHAN_VELOCITYY:
					return new KFVelocityY();
				case CHAN_VELOCITYZ:
					return new KFVelocityZ();
				case CHAN_VELOCITY:
					return new KFVelocity();
				case CHAN_TEXTURE:
					return new KFTexture();
				case CHAN_ROTATION:
					return new KFRotation();
				default:
					return NULL;
			};
		}

		bool Open(const char* path){
			ScopedPointer<File> fh(FILE_SYS()->OpenFile(path, "rb"));
			if(!fh) return false;

			unsigned int count, subCount;
			fh->Read(count);
			mEmitters.SetSize(count);
			for(unsigned int i = 0; i < count; ++i){
				ParticleEmitter* em = &mEmitters[i];
				em->mName = fh->ReadStringLength<unsigned int>();
				fh->Read(em->mLifetime);
				fh->Read(em->mEmitRate);
				fh->Read(em->mLoopCount);
				fh->Read(em->mSpawnDirection);
				fh->Read(em->mEmitRadius);
				fh->Read(em->mGravity);
				em->mTexture = fh->ReadStringLength<unsigned int>();
				fh->Read(em->mParticleCount);
				fh->Read(em->mAlignType);
				fh->Read(em->mUpdateCoordinates);
				fh->Read(em->mTextureWidth);
				fh->Read(em->mTextureHeight);
				fh->Read(em->mSpriteType);
				fh->Read(em->mBlendDest);
				fh->Read(em->mBlendSrc);
				fh->Read(em->mBlendOp);
				fh->Read(subCount);
				em->mKeyFrames.SetSize(subCount);
				for(unsigned int j = 0; j < subCount; ++j){
					IKeyFrame* kf = CreateKeyFrame(fh);
					if(!kf) return false;
					kf->Load(fh);
					em->mKeyFrames[i] = kf;
				}
			}

			LOG("Loaded: '%s' W: %u", path, mEmitters.Size());

			fh->Close();
			return true;
		}

	private:
		Array<ParticleEmitter> mEmitters;
	};
};

#endif