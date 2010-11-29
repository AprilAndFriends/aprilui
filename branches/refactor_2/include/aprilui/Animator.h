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

#include "AprilUIExport.h"

namespace AprilUI
{
	enum AnimationFunction
	{
		Sine,
		Square,
		Saw,
		Triangle,
		Linear,
		Random
	};
	
	class AprilUIExport Animator : public Object
	{
	public:
		Animator(chstr type, chstr name, grect rect);
		
		AnimationFunction getAnimationFunction() { return mFunction; }
		void setAnimationFunction(AnimationFunction value) { mFunction = value; }
		float getTimer() { return mTimer; }
		void setTimer(float value) { mTimer = value; }
		float getDelay() { return mDelay; }
		void setDelay(float value) { mDelay = value; }
		float getPeriods() { return mPeriods; }
		void setPeriods(float value) { mPeriods = value; }
		bool getReset() { return mReset; }
		void setReset(bool value) { mReset = value; }
		virtual bool isAnimated() = 0;
		
		void setProperty(chstr name, chstr value);
		
	protected:
		AnimationFunction mFunction;
		bool mTimer;
		bool mDelay;
		float mPeriods;
		bool mReset;
		
		void update(float k);
		float _calculateStep(float dc_offset, float amplitude, float speed);
		
	};
}

#endif
