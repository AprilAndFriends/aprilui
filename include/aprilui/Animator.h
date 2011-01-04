/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_ANIMATOR_H
#define APRILUI_ANIMATOR_H

#include <gtypes/Rectangle.h>

#include "Object.h"

#include "apriluiExport.h"

namespace aprilui
{
	enum AnimationFunction
	{
		Linear,
		Sine,
		Square,
		Saw,
		Triangle,
		Hover,
		Random
	};
	
	class apriluiExport Animator : public Object
	{
	public:
		Animator(chstr type, chstr name, grect rect);
		
		AnimationFunction getAnimationFunction() { return mFunction; }
		void setAnimationFunction(AnimationFunction value) { mFunction = value; }
		bool getActive() { return mActive; }
		void setActive(bool value) { mActive = value; }
		float getTimer() { return mTimer; }
		void setTimer(float value) { mTimer = value; }
		float getDelay() { return mDelay; }
		void setDelay(float value) { mDelay = value; }
		float getPeriods() { return mPeriods; }
		void setPeriods(float value) { mPeriods = value; }
		float getAmplitude() { return mAmplitude; }
		void setAmplitude(float value) { mAmplitude = value; }
		float getSpeed() { return mSpeed; }
		void setSpeed(float value) { mSpeed = value; }
		float getDcOffset() { return mDcOffset; }
		void setDcOffset(float value) { mDcOffset = value; }
		float getAcceleration() { return mAcceleration; }
		void setAcceleration(float value) { mAcceleration = value; }
		bool getDiscrete() { return mDiscrete; }
		void setDiscrete(bool value) { mDiscrete = value; }
		bool getReset() { return mReset; }
		void setReset(bool value) { mReset = value; }
		bool isAnimated();
		bool isExpired();
		
		void setProperty(chstr name, chstr value);
		
		void update(float k);
		
	protected:
		float mValue;
		AnimationFunction mFunction;
		bool mActive;
		float mTimer;
		float mDelay;
		float mPeriods;
		float mAmplitude;
		float mSpeed;
		float mDcOffset;
		float mAcceleration;
		bool mDiscrete;
		bool mReset;
		
		float _calculateValue(float k);
		
	};
}

#endif
