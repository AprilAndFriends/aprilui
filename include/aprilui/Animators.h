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
			float mDestX,mDestY;
		public:
			Mover(chstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};
		class Scaler : public Object
		{
			float mInitialW,mInitialH,mInitialSW,mInitialSH;
			float mAccelW,mAccelH;
			float mSpeedW,mSpeedH;
			float mDestW,mDestH;
		public:
			Scaler(chstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};


		class Rotator : public Object
		{
			float mInitialSpeed,mInitialAngle,mSpeed,mAccel;
		public:
			Rotator(chstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};

		class AlphaFader : public Object
		{
			float mInitialSpeed,mInitialAlpha,mSpeed,mAccel,mTimer,mDelay,mDestAlpha;
		public:
			AlphaFader(chstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};
		
		class AlphaOscillator : public Object
		{
			float mBaseline,mAmplitude,mSpeed,mTimer,mInitialAlpha;
		public:
			AlphaOscillator(chstr name);

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
			ColorAlternator(chstr name);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};

		class Blinker : public Object
		{
			float mDelay,mDuration,mFrequency,mTimer,mDelayTimer,mDurationTimer;
			bool mStartVisibility,mEndVisibility;
		public:
			Blinker(chstr name);

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
			FrameAnimation(chstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};
		
		class Earthquake : public Object
		{
			float mInitialX,mInitialY;
			float mIntensity,mDuration,mConstDuration,mFreq;
			float mTimer,mFreqTimer;
		public:
			Earthquake(chstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};

	}
}

#endif
