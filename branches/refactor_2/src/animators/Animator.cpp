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
		mActive = true;
		mTimer = 0.0f;
		mDelay = 0.0f;
		mPeriods = -1.0f;
		mAmplitude = 0.5f;
		mSpeed = 1.0f;
		mDcOffset = 0.0f;
		mAcceleration = 0.0f;
		mDiscrete = true;
		mReset = false;
	}
	
	void Animator::update(float k)
	{
		Object::update(k);
		if (!mActive)
		{
			return;
		}
		mDelay -= k;
		if (mDelay > 0.0f)
		{
			return;
		}
		if (k > -mDelay)
		{
			k += mDelay;
		}
		mTimer += k;
		if (fabs(mAcceleration) > 0.01f)
		{
			mSpeed += mAcceleration * k;
		}
	}
	
	float Animator::_calculateValue(float k, float value)
	{
		if (mDelay > 0.0f)
		{
			return (mDiscrete ? (float)(int)mDcOffset : mDcOffset);
		}
		float time = mTimer;
		if (mPeriods >= 0.0f && mTimer * mSpeed > mPeriods)
		{
			if (mReset)
			{
				return (mDiscrete ? (float)(int)mDcOffset : mDcOffset);
			}
			time = mPeriods / mSpeed;
		}
		float result = 0.0f;
		switch (mFunction)
		{
		case AprilUI::Sine:
			result = (float)dsin(time * mSpeed * 360) * mAmplitude;
			break;
		case AprilUI::Square:
			result = (fmod(time * mSpeed, 1.0f) < 0.5f ? mAmplitude : -mAmplitude);
			break;
		case AprilUI::Saw:
			result = (fmod(time * mSpeed + 0.5f, 1.0f) - 0.5f) * 2 * mAmplitude;
			break;
		case AprilUI::Triangle:
			result = fmod(time * mSpeed, 1.0f);
			if (result < 0.25f || result >= 0.75f)
			{
				result = (fmod(time * mSpeed + 0.5f, 1.0f) - 0.5f) * 4 * mAmplitude;
			}
			else
			{
				result = -(fmod(time * mSpeed - 0.25f, 1.0f) - 0.25f) * 4 * mAmplitude;
			}
			break;
		case AprilUI::Linear:
			result = time * mSpeed * mAmplitude;
			break;
		case AprilUI::Hover:
			if (mParent->isCursorInside())
			{
				result = hmin(value - mDcOffset + k * mSpeed, mAmplitude);
			}
			else
			{
				result = hmax(value - mDcOffset - k * mSpeed, -mAmplitude);
			}
			break;
		case AprilUI::Random:
			result = hrandf(-mSpeed * mAmplitude, mSpeed * mAmplitude);
			break;
		}
		return (mDiscrete ? (float)(int)(result + mDcOffset) : (result + mDcOffset));
	}
	
	bool Animator::isAnimated()
	{
		if (!mActive)
		{
			return false;
		}
		if (mFunction == Hover)
		{
			return true;
		}
		if (mDelay > 0.0f)
		{
			return false;
		}
		if (mPeriods >= 0.0f && mTimer * mSpeed > mPeriods)
		{
			return false;
		}
		return true;
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
			else if (value == "hover")		setAnimationFunction(AprilUI::Hover);
			else if (value == "random")		setAnimationFunction(AprilUI::Random);
		}
		else if (name == "active")			setActive(value);
		else if (name == "timer")			setTimer(value);
		else if (name == "delay")			setDelay(value);
		else if (name == "periods")			setPeriods(value);
		else if (name == "amplitude")		setAmplitude(value);
		else if (name == "speed")			setSpeed(value);
		else if (name == "dc_offset")		setDcOffset(value);
		else if (name == "acceleration")	setAcceleration(value);
		else if (name == "discrete")		setDiscrete(value);
		else if (name == "reset")			setReset(value);
	}
	
}
