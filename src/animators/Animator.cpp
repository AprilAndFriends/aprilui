/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "Animator.h"

namespace AprilUI
{
	Animator::Animator(chstr type, chstr name, grect rect) :
		Object(type, name, rect)
	{
		mFunction = AprilUI::Sine;
		mTimer = 0.0f;
		mDelay = 0.0f;
		mPeriods = -1.0f;
		mReset = false;
	}
	
	void Animator::update(float k)
	{
		Object::update(k);
	}
	
	float Animator::_calculateStep(float dcOffset, float amplitude, float speed)
	{
		if (mDelay > 0.0f)
		{
			return dcOffset;
		}
		if (mPeriods >= 0.0f && mTimer * speed > mPeriods)
		{
			if (mReset)
			{
				return dcOffset;
			}
			speed = mPeriods / mTimer;
		}
		float result;
		switch (mFunction)
		{
		case AprilUI::Sine:
			result = dsin(mTimer * speed * 360) * amplitude;
			break;
		case AprilUI::Square:
			//(-((int)(mTimer * speed) % 2) * 2 + 1)
			result = (-((int)(mTimer * speed) % 2) * 2 + 1) * amplitude;
			break;
		case AprilUI::Saw:
			result = fmod(mTimer * speed, 1.0f) * amplitude;
			break;
		case AprilUI::Triangle:
			if ((int)(mTimer * speed) % 2 == 0)
			{
				result = fmod(mTimer * speed, 1.0f) * amplitude;
			}
			else
			{
				result = (1.0f - fmod(mTimer * speed, 1.0f)) * amplitude;
			}
			break;
		case AprilUI::Linear:
			result = mTimer * speed * amplitude;
			break;
		}
		return (result + dcOffset);
	}
	
	void Animator::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
		if      (name == "function")
		{
			if      (value == "sine")		setAnimationFunction(AprilUI::Sine);
			else if (value == "saw")		setAnimationFunction(AprilUI::Saw);
			else if (value == "square")		setAnimationFunction(AprilUI::Square);
			else if (value == "triangle")	setAnimationFunction(AprilUI::Triangle);
			else if (value == "linear")		setAnimationFunction(AprilUI::Linear);
		}
		else if (name == "timer")	setTimer(value);
		else if (name == "delay")	setDelay(value);
	}
	
}
