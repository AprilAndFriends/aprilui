#include <math.h>
#include "Animators.h"
#include "Util.h"
#include "Image.h"
#include <stdlib.h>

namespace AprilUI
{
	namespace Animators
	{
		Mover::Mover(std::string name) : Object("Animators::Mover",name,0,0,1,1)
		{
			mAccelX=mAccelY=mSpeedX=mSpeedY=mInitialSY=mInitialSX=0;
			mInitialX=mInitialY=-10000;
		}

		void Mover::setProperty(const std::string& name,const std::string& value)
		{
			if      (name == "speed_x") mSpeedX=mInitialSX=str_to_float(value);
			else if (name == "speed_y") mSpeedY=mInitialSY=str_to_float(value);
			else if (name == "accel_x") mAccelX=str_to_float(value);
			else if (name == "accel_y") mAccelY=str_to_float(value);
		}

		void Mover::notifyEvent(const std::string& event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				if (mInitialX < -9000)
				{
					mInitialX=mParent->getXPosition();
					mInitialY=mParent->getYPosition();
				}
				else
				{
					mParent->setPosition(mInitialX,mInitialY);
				}
				mSpeedX=mInitialSX;
				mSpeedY=mInitialSY;
			}
		}

		void Mover::update(float k)
		{
			gtypes::Vector2 v=mParent->getPosition();
			if (fabs(mAccelX) > 0.01f || fabs(mAccelY) > 0.01f)
			{
				mSpeedX+=mAccelX*k;
				mSpeedY+=mAccelY*k;
			}

			v.x+=k*mSpeedX;
			v.y+=k*mSpeedY;
			
			mParent->setPosition(v);
		}



		Scaler::Scaler(std::string name) : Object("Animators::Scaler",name,0,0,1,1)
		{
			mAccelW=mAccelH=mSpeedW=mSpeedH=mInitialW=mInitialH=0;
			mInitialW=mInitialH=-10000;
		}

		void Scaler::setProperty(const std::string& name,const std::string& value)
		{
			if      (name == "speed_w") mSpeedW=mInitialSW=str_to_float(value);
			else if (name == "speed_h") mSpeedH=mInitialSH=str_to_float(value);
			else if (name == "accel_w") mAccelW=str_to_float(value);
			else if (name == "accel_h") mAccelH=str_to_float(value);
		}

		void Scaler::notifyEvent(const std::string& event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				if (mInitialW < -9000)
				{
					mInitialW=mParent->getWidth();
					mInitialH=mParent->getHeight();
				}
				else
				{
					mParent->setSize(mInitialW,mInitialH);
				}
				mSpeedH=mInitialSH;
				mSpeedW=mInitialSW;
			}
		}

		void Scaler::update(float k)
		{
			gtypes::Vector2 v=mParent->getSize();
			if (fabs(mAccelW) > 0.01f || fabs(mAccelH) > 0.01f)
			{
				mSpeedW+=mAccelW*k;
				mSpeedH+=mAccelH*k;
			}

			v.x+=k*mSpeedW;
			v.y+=k*mSpeedH;

			mParent->setSize(v.x,v.y);
		}

		Rotator::Rotator(std::string name) : Object("Animators::Scaler",name,0,0,1,1)
		{
			mAccel=mSpeed=0;
			mInitialSpeed=-10000;
			mInitialAngle=-10000001;
		}

		void Rotator::setProperty(const std::string& name,const std::string& value)
		{
			if      (name == "speed") mSpeed=mInitialSpeed=str_to_float(value);
			else if (name == "accel") mAccel=str_to_float(value);
		}

		void Rotator::notifyEvent(const std::string& event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				if (mInitialAngle < -1000000)
					mInitialAngle=((RotationImageBox*) mParent)->getAngle();
				else
					((RotationImageBox*) mParent)->setAngle(mInitialAngle);
				mSpeed=mInitialSpeed;
			}
		}

		void Rotator::update(float k)
		{
			float angle=((RotationImageBox*) mParent)->getAngle();
			if (fabs(mAccel) > 0.01f)
			{
				mSpeed+=mAccel*k;
			}
			angle+=k*mSpeed;
			
			((RotationImageBox*) mParent)->setAngle(angle);
		}



		AlphaFader::AlphaFader(std::string name) : Object("Animators::Scaler",name,0,0,1,1)
		{
			mAccel=mSpeed=mInitialSpeed=mDelay=mTimer=0;
			mInitialAlpha=-10001;
		}

		void AlphaFader::setProperty(const std::string& name,const std::string& value)
		{
			if      (name == "speed") mSpeed=mInitialSpeed=str_to_float(value);
			else if (name == "accel") mAccel=str_to_float(value);
			else if (name == "delay") mDelay=str_to_float(value);
		}

		void AlphaFader::notifyEvent(const std::string& event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				if (mInitialAlpha < -10000)
					mInitialAlpha=mParent->getAlpha();
				else
					mParent->setAlpha(mInitialAlpha);
					
				if (mDelay) mTimer=mDelay;
				mSpeed=mInitialSpeed;
			}
		}

		void AlphaFader::update(float k)
		{
			if (mTimer > 0) { mTimer-=k; return; }
			float alpha=mParent->getAlpha();
			if (fabs(mAccel) > 0.01f)
				mSpeed+=mAccel*k;
			alpha+=k*mSpeed;

			mParent->setAlpha(std::max(0.0f,std::min(1.0f,alpha)));
		}



		ColorAlternator::ColorAlternator(std::string name) : Object("Animators::ColorAlternator",name,0,0,1,1)
		{
			mLow[0]=mLow[1]=mLow[2]=mLow[3]=0;
			mHigh[0]=mHigh[1]=mHigh[2]=mHigh[3]=1;
			mTimer=0;
			mSpeed=2;
		}

		void ColorAlternator::setProperty(const std::string& name,const std::string& value)
		{
			if (name == "low_color")       hexstr_to_argb_float(value,&mLow[0],&mLow[1],&mLow[2],&mLow[3]);
			else if (name == "high_color") hexstr_to_argb_float(value,&mHigh[0],&mHigh[1],&mHigh[2],&mHigh[3]);
			else if (name == "speed")      mSpeed=str_to_float(value);
		}

		void ColorAlternator::update(float k)
		{
			ColoredImage* img=0;
			ImageBox* parent=dynamic_cast<ImageBox*>(mParent);
			if (parent) img=dynamic_cast<ColoredImage*>(parent->getImage());
			if (!img) return;

			mTimer+=k*mSpeed;

			float a=(sin(mTimer)+1)*(mHigh[0]-mLow[0])/2+mLow[0];
			float r=(sin(mTimer)+1)*(mHigh[1]-mLow[1])/2+mLow[1];
			float g=(sin(mTimer)+1)*(mHigh[2]-mLow[2])/2+mLow[2];
			float b=(sin(mTimer)+1)*(mHigh[3]-mLow[3])/2+mLow[3];

			img->setColor(a,r,g,b);
		}
		
		Blinker::Blinker(std::string name) : Object("Animators::Blinker",name,0,0,1,1)
		{
			mDelay=mDuration=mTimer=mDelayTimer=mDurationTimer=0;
			mStartVisibility=mEndVisibility=0;
			mFrequency=100;
		}

		void Blinker::setProperty(const std::string& name,const std::string& value)
		{
			if      (name == "delay")    mDelay=str_to_float(value);
			else if (name == "duration") mDuration=str_to_float(value);
			else if (name == "freq")     mFrequency=str_to_float(value);
			else if (name == "start_visibility") mStartVisibility=str_to_int(value)!=0;
			else if (name == "end_visibility")   mEndVisibility=str_to_int(value)!=0;

		}

		void Blinker::notifyEvent(const std::string& event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				mDelayTimer=mDelay;
				mDurationTimer=mDuration;
				mParent->setVisible(mStartVisibility);
			}
		}

		void Blinker::update(float k)
		{
			if (mDelayTimer > 0) mDelayTimer-=k;
			else if (mDuration >= 0)
			{
				mTimer-=k;
				if (mTimer < 0)
				{
					mParent->setVisible(!mParent->isVisible());
					mTimer=((rand()%10000)/10000.0f)/mFrequency;
				}
				mDuration-=k;
				if (mDuration < 0)
				{
					mParent->setVisible(mEndVisibility);
				}
			}
		}

		FrameAnimation::FrameAnimation(std::string name) : Object("Animators::FrameAnimation",name,0,0,1,1)
		{
			mStartFrame=0; mEndFrame=100;
			mAnimationTime=10;
			mTimer=0;
		}

		void FrameAnimation::setProperty(const std::string& name,const std::string& value)
		{
			if      (name == "start_frame") mStartFrame=str_to_int(value);
			else if (name == "end_frame")   mEndFrame=str_to_int(value);
			else if (name == "time")        mAnimationTime=str_to_float(value);
			else if (name == "base_name")   mImageBaseName=value;
		}

		void FrameAnimation::notifyEvent(const std::string& event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				mTimer=0;
			}
		}

		void FrameAnimation::update(float k)
		{
			mTimer+=k;
			if (mTimer >= mAnimationTime) mTimer-=mAnimationTime;
			int frame=mStartFrame+(int) ((mEndFrame-mStartFrame)*mTimer/mAnimationTime);
			ImageBox* img=dynamic_cast<ImageBox*>(mParent);
			if (img)
			{
				img->setImageByName(mImageBaseName+str(frame));
			}
			else writelog("Animators::FrameAnimation: parent object not a subclass of Objects::ImageBox!");
		}
	}
}
