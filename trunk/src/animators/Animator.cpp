/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 1.91
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "Animator.h"
#include "aprilui.h"

namespace aprilui
{
	Animator::Animator(chstr name) :
		Object(name, grect(0, 0, 1, 1))
	{
		this->mTimeSinceLastFrame = 0.0f;
		this->mValue = 0.0f;
		this->mFunction = aprilui::Animator::Linear;
		this->mTimer = 0.0f;
		this->mDelay = 0.0f;
		this->mPeriods = 1.0f;
		this->mAmplitude = 0.5f;
		this->mSpeed = 1.0f;
		this->mOffset = 0.0f;
		this->mAcceleration = 0.0f;
		this->mDiscreteStep = 0;
		this->mReset = false;
		this->mInheritValue = false;
		this->mTarget = 0.0f;
		this->mUseTarget = false;
		this->mCustomFunction = NULL;
	}
	
	Animator::~Animator()
	{
	}

	void Animator::update(float k)
	{
		this->mTimeSinceLastFrame = k;
		Object::update(this->mTimeSinceLastFrame);
		if (!this->mEnabled)
		{
			return;
		}
		if (this->mDelay > 0.0f)
		{
			this->mDelay -= this->mTimeSinceLastFrame;
			if (this->mDelay > 0.0f)
			{
				return;
			}
			this->notifyEvent("OnDelayEnd", NULL);
			this->mTimeSinceLastFrame += this->mDelay;
		}
		this->mTimer += this->mTimeSinceLastFrame;
		if (!heqf(this->mAcceleration, 0.0f, (float)HL_E_TOLERANCE))
		{
			this->mSpeed += this->mAcceleration * this->mTimeSinceLastFrame;
		}
	}
	
	bool Animator::_checkUpdate(float k)
	{
		float delay = this->mDelay;
		bool animated = this->isAnimated();
		Animator::update(k);
		return (animated || this->isAnimated() || delay > 0.0f && this->mDelay <= 0.0f);
	}
	
	float Animator::_calculateValue(float k)
	{
		if (this->mDelay > 0.0f)
		{
			return (this->mDiscreteStep != 0 ? (float)((int)(this->mOffset / this->mDiscreteStep) * this->mDiscreteStep) : this->mOffset);
		}
		float time = this->mTimer;
		if (this->isExpired())
		{
			if (this->mReset)
			{
				return (this->mDiscreteStep != 0 ? (float)((int)(this->mOffset / this->mDiscreteStep) * this->mDiscreteStep) : this->mOffset);
			}
			time = this->mPeriods / habs(this->mSpeed);
		}
		float result = 0.0f;
		switch (this->mFunction)
		{
		case aprilui::Animator::Linear:
			result = time * this->mSpeed * this->mAmplitude;
			break;
		case aprilui::Animator::Sine:
			result = (float)dsin(time * this->mSpeed * 360) * this->mAmplitude;
			break;
		case aprilui::Animator::Square:
			result = (fmod(time * this->mSpeed, 1.0f) < 0.5f ? this->mAmplitude : -this->mAmplitude);
			break;
		case aprilui::Animator::Saw:
			result = (fmod(time * this->mSpeed + 0.5f, 1.0f) - 0.5f) * 2 * this->mAmplitude;
			break;
		case aprilui::Animator::Triangle:
			result = fmod(time * this->mSpeed, 1.0f);
			if (!is_in_range(result, 0.25f, 0.75f))
			{
				result = (fmod(time * this->mSpeed + 0.5f, 1.0f) - 0.5f) * 4 * this->mAmplitude;
			}
			else
			{
				result = -(fmod(time * this->mSpeed - 0.25f, 1.0f) - 0.25f) * 4 * this->mAmplitude;
			}
			break;
		case aprilui::Animator::Random:
			result = hrandf(-this->mSpeed * this->mAmplitude, this->mSpeed * this->mAmplitude);
			break;
		case aprilui::Animator::Hover:
			if ((this->mAmplitude >= 0.0f) == this->mParent->isCursorInside())
			{
				result = hmin(this->mValue - this->mOffset + k * this->mSpeed, (float)habs(this->mAmplitude));
			}
			else
			{
				result = hmax(this->mValue - this->mOffset - k * this->mSpeed, -(float)habs(this->mAmplitude));
			}
			break;
		case aprilui::Animator::Custom:
			result = (this->mCustomFunction != NULL ? this->mCustomFunction(this, time) : this->mValue);
			break;
		}
		return (this->mDiscreteStep != 0 ? (float)((int)((result + this->mOffset) / this->mDiscreteStep) * this->mDiscreteStep) : (result + this->mOffset));
	}
	
	bool Animator::isAnimated()
	{
		if (!this->mEnabled)
		{
			return false;
		}
		if (this->mFunction == Hover)
		{
			return true;
		}
		if (this->mDelay > 0.0f)
		{
			return false;
		}
		if (this->isExpired())
		{
			return false;
		}
		return true;
	}
	
	bool Animator::isWaitingAnimation()
	{
		if (!this->mEnabled)
		{
			return false;
		}
		if (this->mFunction == Hover)
		{
			return true;
		}
		if (this->isExpired())
		{
			return false;
		}
		return true;
	}
	
	bool Animator::isExpired()
	{
		return (!this->mEnabled || this->mPeriods >= 0.0f && this->mTimer * habs(this->mSpeed) > this->mPeriods);
	}
	
	void Animator::setTime(float value)
	{
		this->mSpeed = 1.0f / value;
	}
	
	bool Animator::setProperty(chstr name, chstr value)
	{
		if		(name == "function" || name == "func")
		{
			if		(value == "sine")		this->setAnimationFunction(aprilui::Animator::Sine);
			else if	(value == "saw")		this->setAnimationFunction(aprilui::Animator::Saw);
			else if	(value == "square")		this->setAnimationFunction(aprilui::Animator::Square);
			else if	(value == "triangle")	this->setAnimationFunction(aprilui::Animator::Triangle);
			else if	(value == "linear")		this->setAnimationFunction(aprilui::Animator::Linear);
			else if	(value == "random")		this->setAnimationFunction(aprilui::Animator::Random);
			else if	(value == "hover")		this->setAnimationFunction(aprilui::Animator::Hover);
			else if	(value == "custom")		this->setAnimationFunction(aprilui::Animator::Custom);
		}
		else if	(name == "timer")						this->setTimer(value);
		else if	(name == "delay")						this->setDelay(value);
		else if	(name == "periods")						this->setPeriods(value);
		else if	(name == "amplitude" || name == "amp")	this->setAmplitude(value);
		else if	(name == "peak_to_peak")				this->setAmplitude((float)value / 2.0f);
		else if	(name == "speed")						this->setSpeed(value);
		else if	(name == "offset")						this->setOffset(value);
		else if	(name == "dc_offset")
		{
			aprilui::log("WARNING: 'dc_offset=' is deprecated, use 'offset=' instead!"); // DEPRECATED
			this->setOffset(value);
		}
		else if	(name == "acceleration")	this->setAcceleration(value);
		else if	(name == "discrete")
		{
			aprilui::log("WARNING: 'discrete=' is deprecated, use 'discrete_step=' instead!"); // DEPRECATED
			this->setDiscreteStep((bool)value ? 0 : 1);
		}
		else if	(name == "discrete_step")	this->setDiscreteStep(value);
		else if	(name == "reset")			this->setReset(value);
		else if	(name == "inherit_value")	this->setInheritValue(value);
		// derived values
		else if	(name == "target")
		{
			this->setTarget(value);
			this->setUseTarget(true);
			this->setInheritValue(true);
		}
		else if	(name == "time")			this->setTime(value);
		else return Object::setProperty(name, value);
		return true;
	}
	
}
