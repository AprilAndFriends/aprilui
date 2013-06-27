/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.65
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "Animator.h"
#include "aprilui.h"

namespace aprilui
{
	Object::AnimationFunction Animator::Linear = Object::AnimationFunction::Linear; // DEPRECATED
	Object::AnimationFunction Animator::Sine = Object::AnimationFunction::Sine; // DEPRECATED
	Object::AnimationFunction Animator::Square = Object::AnimationFunction::Square; // DEPRECATED
	Object::AnimationFunction Animator::Saw = Object::AnimationFunction::Saw; // DEPRECATED
	Object::AnimationFunction Animator::Triangle = Object::AnimationFunction::Triangle; // DEPRECATED
	Object::AnimationFunction Animator::Random = Object::AnimationFunction::Random; // DEPRECATED
	Object::AnimationFunction Animator::Hover = Object::AnimationFunction::Hover; // DEPRECATED
	Object::AnimationFunction Animator::Custom = Object::AnimationFunction::Custom; // DEPRECATED

	Animator::Animator(chstr name) :
		Object(name, grect(0, 0, 1, 1))
	{
		this->mTimeSinceLastFrame = 0.0f;
		this->mValue = 0.0f;
		this->mFunction = aprilui::Object::Linear;
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

	void Animator::OnDrawDebug()
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
		case Object::Linear:
			result = time * this->mSpeed * this->mAmplitude;
			break;
		case Object::Sine:
			result = (float)dsin(time * this->mSpeed * 360) * this->mAmplitude;
			break;
		case Object::Square:
			result = (hmodf(time * this->mSpeed, 1.0f) < 0.5f ? this->mAmplitude : -this->mAmplitude);
			break;
		case Object::Saw:
			result = (hmodf(time * this->mSpeed + 0.5f, 1.0f) - 0.5f) * 2 * this->mAmplitude;
			break;
		case Object::Triangle:
			result = hmodf(time * this->mSpeed, 1.0f);
			if (!is_in_range(result, 0.25f, 0.75f))
			{
				result = (hmodf(time * this->mSpeed + 0.5f, 1.0f) - 0.5f) * 4 * this->mAmplitude;
			}
			else
			{
				result = -(hmodf(time * this->mSpeed - 0.25f, 1.0f) - 0.25f) * 4 * this->mAmplitude;
			}
			break;
		case Object::Random:
			result = hrandf(-this->mSpeed * this->mAmplitude, this->mSpeed * this->mAmplitude);
			break;
		case Object::Hover:
			if ((this->mAmplitude >= 0.0f) == this->mParent->isCursorInside())
			{
				result = hmin(this->mValue - this->mOffset + k * this->mSpeed, (float)habs(this->mAmplitude));
			}
			else
			{
				result = hmax(this->mValue - this->mOffset - k * this->mSpeed, -(float)habs(this->mAmplitude));
			}
			break;
		case Object::Custom:
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
		if (this->mFunction == Object::Hover)
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
		if (this->mFunction == Object::Hover)
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
		if (value > 0.0f)
		{
			this->mSpeed = 1.0f / value;
		}
		else
		{
			hlog::warn(aprilui::logTag, "Cannot set \"time\" to 0 or less.");
		}
	}
	
	hstr Animator::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if		(name == "function" || name == "func")
		{
			if		(mFunction == Object::Sine)		return "sine";
			else if	(mFunction == Object::Saw)		return "saw";
			else if	(mFunction == Object::Square)	return "square";
			else if	(mFunction == Object::Triangle)	return "triangle";
			else if	(mFunction == Object::Linear)	return "linear";
			else if	(mFunction == Object::Random)	return "random";
			else if	(mFunction == Object::Hover)	return "hover";
			else									return "custom";
		}
		else if	(name == "timer")			return this->getTimer();
		else if	(name == "delay")			return this->getDelay();
		else if	(name == "periods")			return this->getPeriods();
		else if	(name == "amplitude")		return this->getAmplitude();
		else if	(name == "amp")
		{
			hlog::warn(aprilui::logTag, "\"amp\" is deprecated. Use \"amplitude\" instead."); // DEPRECATED
			return this->getAmplitude();
		}
		else if	(name == "peak_to_peak")	return (2 * this->getAmplitude());
		else if	(name == "speed")			return this->getSpeed();
		else if	(name == "offset")			return this->getOffset();
		else if	(name == "acceleration")	return this->getAcceleration();
		else if	(name == "discrete_step")	return this->getDiscreteStep();
		else if	(name == "reset")			return this->isReset();
		else if	(name == "inherit_value")	return this->getInheritValue();
		// derived values
		else if	(name == "target")
		{
			return this->getTarget();
		}
		return Object::getProperty(name, propertyExists);
	}
	
	bool Animator::setProperty(chstr name, chstr value)
	{
		if		(name == "function" || name == "func")
		{
			if		(value == "sine")		this->setAnimationFunction(Object::Sine);
			else if	(value == "saw")		this->setAnimationFunction(Object::Saw);
			else if	(value == "square")		this->setAnimationFunction(Object::Square);
			else if	(value == "triangle")	this->setAnimationFunction(Object::Triangle);
			else if	(value == "linear")		this->setAnimationFunction(Object::Linear);
			else if	(value == "random")		this->setAnimationFunction(Object::Random);
			else if	(value == "hover")		this->setAnimationFunction(Object::Hover);
			else if	(value == "custom")		this->setAnimationFunction(Object::Custom);
		}
		else if	(name == "timer")			this->setTimer(value);
		else if	(name == "delay")			this->setDelay(value);
		else if	(name == "periods")			this->setPeriods(value);
		else if	(name == "amplitude")		this->setAmplitude(value);
		else if	(name == "amp")
		{
			hlog::warn(aprilui::logTag, "\"amp=\" is deprecated. Use \"amplitude=\" instead."); // DEPRECATED
			this->setAmplitude(value);
		}
		else if	(name == "peak_to_peak")	this->setAmplitude((float)value * 0.5f);
		else if	(name == "speed")			this->setSpeed(value);
		else if	(name == "offset")			this->setOffset(value);
		else if	(name == "acceleration")	this->setAcceleration(value);
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
	
	void Animator::notifyEvent(chstr name, void* params)
	{
		if (name == "AttachToObject" || name == "OnDelayEnd" && this->mInheritValue)
		{
			this->mValue = this->mOffset = this->_getObjectValue();
			if (this->mUseTarget)
			{
				this->mAmplitude = this->mTarget - this->mValue;
			}
		}
		Object::notifyEvent(name, params);
	}
		
	void Animator::_valueUpdateSimple(float k)
	{
		if (this->_checkUpdate(k))
		{
			this->mValue = this->_getObjectValue(); // required because this->_calculateValue may use mValue
			this->mValue = this->_calculateValue(this->mTimeSinceLastFrame);
			this->_setObjectValue(this->mValue);
		}
	}
	
	void Animator::_valueUpdateUChar(float k)
	{
		if (this->_checkUpdate(k))
		{
			this->mValue = hclamp(this->_calculateValue(this->mTimeSinceLastFrame), 0.0f, 255.0f);
			if ((unsigned char)this->mValue != (unsigned char)this->_getObjectValue())
			{
				this->_setObjectValue(this->mValue);
			}
		}
	}
	
}
