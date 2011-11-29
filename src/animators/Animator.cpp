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
	Animator::Animator(chstr name) :
		Object(name, grect(0, 0, 1, 1))
	{
		mTimeSinceLastFrame = 0.0f;
		mValue = 0.0f;
		mFunction = aprilui::Animator::Linear;
		mTimer = 0.0f;
		mDelay = 0.0f;
		mPeriods = 1.0f;
		mAmplitude = 0.5f;
		mSpeed = 1.0f;
		mOffset = 0.0f;
		mAcceleration = 0.0f;
		mDiscreteStep = 0;
		mReset = false;
		mInheritValue = false;
		mTarget = 0.0f;
		mUseTarget = false;
		mCustomFunction = NULL;
	}
	
	void Animator::update(float k)
	{
		mTimeSinceLastFrame = k;
		Object::update(mTimeSinceLastFrame);
		if (!mEnabled)
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
			notifyEvent("OnDelayEnd", NULL);
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
			return (mDiscreteStep != 0 ? (float)((int)(mOffset / mDiscreteStep) * mDiscreteStep) : mOffset);
		}
		float time = mTimer;
		if (isExpired())
		{
			if (mReset)
			{
				return (mDiscreteStep != 0 ? (float)((int)(mOffset / mDiscreteStep) * mDiscreteStep) : mOffset);
			}
			time = mPeriods / fabs(mSpeed);
		}
		float result = 0.0f;
		switch (mFunction)
		{
		case aprilui::Animator::Linear:
			result = time * mSpeed * mAmplitude;
			break;
		case aprilui::Animator::Sine:
			result = (float)dsin(time * mSpeed * 360) * mAmplitude;
			break;
		case aprilui::Animator::Square:
			result = (fmod(time * mSpeed, 1.0f) < 0.5f ? mAmplitude : -mAmplitude);
			break;
		case aprilui::Animator::Saw:
			result = (fmod(time * mSpeed + 0.5f, 1.0f) - 0.5f) * 2 * mAmplitude;
			break;
		case aprilui::Animator::Triangle:
			result = fmod(time * mSpeed, 1.0f);
			if (!is_in_range(result, 0.25f, 0.75f))
			{
				result = (fmod(time * mSpeed + 0.5f, 1.0f) - 0.5f) * 4 * mAmplitude;
			}
			else
			{
				result = -(fmod(time * mSpeed - 0.25f, 1.0f) - 0.25f) * 4 * mAmplitude;
			}
			break;
		case aprilui::Animator::Random:
			result = hrandf(-mSpeed * mAmplitude, mSpeed * mAmplitude);
			break;
		case aprilui::Animator::Hover:
			if ((mAmplitude >= 0.0f) == mParent->isCursorInside())
			{
				result = hmin(mValue - mOffset + k * mSpeed, (float)fabs(mAmplitude));
			}
			else
			{
				result = hmax(mValue - mOffset - k * mSpeed, -(float)fabs(mAmplitude));
			}
			break;
		case aprilui::Animator::Custom:
			result = (mCustomFunction != NULL ? mCustomFunction(this, time) : mValue);
			break;
		}
		return (mDiscreteStep != 0 ? (float)((int)((result + mOffset) / mDiscreteStep) * mDiscreteStep) : (result + mOffset));
	}
	
	bool Animator::isAnimated()
	{
		if (!mEnabled)
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
	
	bool Animator::isWaitingAnimation()
	{
		if (!mEnabled)
		{
			return false;
		}
		if (mFunction == Hover)
		{
			return true;
		}
		if (isExpired())
		{
			return false;
		}
		return true;
	}
	
	bool Animator::isExpired()
	{
		return (!mEnabled || mPeriods >= 0.0f && mTimer * fabs(mSpeed) > mPeriods);
	}
	
	void Animator::setTime(float value)
	{
		mSpeed = 1.0f / value;
	}
	
	bool Animator::setProperty(chstr name, chstr value)
	{
		if      (name == "function" || name == "func")
		{
			if      (value == "sine")		setAnimationFunction(aprilui::Animator::Sine);
			else if (value == "saw")		setAnimationFunction(aprilui::Animator::Saw);
			else if (value == "square")		setAnimationFunction(aprilui::Animator::Square);
			else if (value == "triangle")	setAnimationFunction(aprilui::Animator::Triangle);
			else if (value == "linear")		setAnimationFunction(aprilui::Animator::Linear);
			else if (value == "random")		setAnimationFunction(aprilui::Animator::Random);
			else if (value == "hover")		setAnimationFunction(aprilui::Animator::Hover);
			else if (value == "custom")		setAnimationFunction(aprilui::Animator::Custom);
		}
		else if (name == "timer")						setTimer(value);
		else if (name == "delay")						setDelay(value);
		else if (name == "periods")						setPeriods(value);
		else if (name == "amplitude" || name == "amp")	setAmplitude(value);
		else if (name == "peak_to_peak")				setAmplitude((float)value / 2.0f);
		else if (name == "speed")						setSpeed(value);
		else if (name == "offset")						setOffset(value);
		else if (name == "dc_offset")
		{
			aprilui::log("WARNING: \"dc_offset=\" is deprecated, use \"offset=\" instead"); // DEPRECATED
			setOffset(value);
		}
		else if (name == "acceleration")	setAcceleration(value);
		else if (name == "discrete")
		{
			aprilui::log("WARNING: \"discrete=\" is deprecated, use \"discrete_step=\" instead"); // DEPRECATED
			setDiscreteStep((bool)value ? 0 : 1);
		}
		else if (name == "discrete_step")	setDiscreteStep(value);
		else if (name == "reset")			setReset(value);
		else if (name == "inherit_value")	setInheritValue(value);
		// derived values
		else if (name == "target")
		{
			setTarget(value);
			setUseTarget(true);
			setInheritValue(true);
		}
		else if (name == "time")			setTime(value);
        else return Object::setProperty(name, value);
        return true;
	}
	
}
