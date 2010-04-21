/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef ANIMATORS_H
#define ANIMATORS_H

#include "Objects.h"

/*******************************************************************************/
namespace AprilUI
{
	namespace Animators
	{ // dummy objects who perform operations on their parents
		class Mover : public Object
		{
			float mInitialX,mInitialY,mInitialSX,mInitialSY;
			float mAccelX,mAccelY;
			float mSpeedX,mSpeedY;
		public:
			Mover(hstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};
		class Scaler : public Object
		{
			float mInitialW,mInitialH,mInitialSW,mInitialSH;
			float mAccelW,mAccelH;
			float mSpeedW,mSpeedH;
		public:
			Scaler(hstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};


		class Rotator : public Object
		{
			float mInitialSpeed,mInitialAngle,mSpeed,mAccel;
		public:
			Rotator(hstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};

		class AlphaFader : public Object
		{
			float mInitialSpeed,mInitialAlpha,mSpeed,mAccel,mTimer,mDelay;
		public:
			AlphaFader(hstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};


		// note: only works on ColoredImages
		class ColorAlternator : public Object
		{
			float mLow[4],mHigh[4];
			float mTimer,mSpeed;
		public:
			ColorAlternator(hstr name);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};

		class Blinker : public Object
		{
			float mDelay,mDuration,mFrequency,mTimer,mDelayTimer,mDurationTimer;
			bool mStartVisibility,mEndVisibility;
		public:
			Blinker(hstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};
		
		class FrameAnimation : public Object
		{
			hstr mImageBaseName;
			int mStartFrame,mEndFrame;
			float mAnimationTime,mTimer;
		public:
			FrameAnimation(hstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};
	}
}

#endif
