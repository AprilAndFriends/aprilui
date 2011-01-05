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
#include "aprilui.h"

namespace aprilui
{
	Animator::Animator(chstr type, chstr name, grect rect) :
		Object(type, name, rect)
	{
		mTimeSinceLastFrame = 0.0f;
		mValue = 0.0f;
		mFunction = aprilui::Linear;
		mActive = true;
		mTimer = 0.0f;
		mDelay = 0.0f;
		mPeriods = -1.0f;
		mAmplitude = 0.5f;
		mSpeed = 1.0f;
		mOffset = 0.0f;
		mAcceleration = 0.0f;
		mDiscrete = false;
		mReset = false;
		mInheritValue = false;
	}
	
	void Animator::update(float k)
	{
		mTimeSinceLastFrame = k;
		Object::update(mTimeSinceLastFrame);
		if (!mActive)
		{
			return;
		}
		if (mDelay > 0.0f)
		{
			mDelay -= mTimeSinceLastFrame;
			if (mDelay > 0.0f)
			{
				return;
			}
			if (mInheritValue)
			{
				notifyEvent("InheritValue", NULL);
			}
			mTimeSinceLastFrame += mDelay;
		}
		mTimer += mTimeSinceLastFrame;
		if (fabs(mAcceleration) > 0.01f)
		{
			mSpeed += mAcceleration * mTimeSinceLastFrame;
		}
	}
	
	float Animator::_calculateValue(float k)
	{
		if (mDelay > 0.0f)
		{
			return (mDiscrete ? (float)(int)mOffset : mOffset);
		}
		float time = mTimer;
		float speed = fabs(mSpeed);
		if (isExpired())
		{
			if (mReset)
			{
				return (mDiscrete ? (float)(int)mOffset : mOffset);
			}
			time = mPeriods / fabs(mSpeed);
		}
		float result = 0.0f;
		switch (mFunction)
		{
		case aprilui::Linear:
			result = time * mSpeed * mAmplitude;
			break;
		case aprilui::Sine:
			result = (float)dsin(time * mSpeed * 360) * mAmplitude;
			break;
		case aprilui::Square:
			result = (fmod(time * mSpeed, 1.0f) < 0.5f ? mAmplitude : -mAmplitude);
			break;
		case aprilui::Saw:
			result = (fmod(time * mSpeed + 0.5f, 1.0f) - 0.5f) * 2 * mAmplitude;
			break;
		case aprilui::Triangle:
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
		case aprilui::Hover:
			if (mParent->isCursorInside())
			{
				result = hmin(mValue - mOffset + k * mSpeed, mAmplitude);
			}
			else
			{
				result = hmax(mValue - mOffset - k * mSpeed, -mAmplitude);
			}
			break;
		case aprilui::Random:
			result = hrandf(-mSpeed * mAmplitude, mSpeed * mAmplitude);
			break;
		}
		return (mDiscrete ? (float)(int)(result + mOffset) : (result + mOffset));
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
		if (isExpired())
		{
			return false;
		}
		return true;
	}
	
	bool Animator::isExpired()
	{
		return (mPeriods >= 0.0f && mTimer * fabs(mSpeed) > mPeriods);
	}
	
	void Animator::setTime(float value)
	{
		mSpeed = 1.0f / value;
	}
	
	void Animator::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
		if      (name == "function")
		{
			if      (value == "sine")		setAnimationFunction(aprilui::Sine);
			else if (value == "saw")		setAnimationFunction(aprilui::Saw);
			else if (value == "square")		setAnimationFunction(aprilui::Square);
			else if (value == "triangle")	setAnimationFunction(aprilui::Triangle);
			else if (value == "linear")		setAnimationFunction(aprilui::Linear);
			else if (value == "hover")		setAnimationFunction(aprilui::Hover);
			else if (value == "random")		setAnimationFunction(aprilui::Random);
		}
		else if (name == "active")			setActive(value);
		else if (name == "timer")			setTimer(value);
		else if (name == "delay")			setDelay(value);
		else if (name == "periods")			setPeriods(value);
		else if (name == "amplitude")		setAmplitude(value);
		else if (name == "speed")			setSpeed(value);
		else if (name == "offset")			setOffset(value);
		else if (name == "dc_offset")
		{
			aprilui::log("Warning: dc_offset is deprecated, use offset instead");
			setOffset(value);
		}
		else if (name == "acceleration")	setAcceleration(value);
		else if (name == "discrete")		setDiscrete(value);
		else if (name == "reset")			setReset(value);
		else if (name == "inherit_value")	setInheritValue(value);
		// derived values
		else if (name == "time")			setTime(value);
	}
	
}
